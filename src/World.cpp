
#include "World.h"
#include "Player.h"
#include "WorldObject.h"

#include <fstream>
#include <algorithm>


#include <hgesprite.h>

World::World( Player *plr )
    : m_player( plr ), m_fog(), m_pauseFlag( false ),
      m_levelGoalReached( false )
{

    m_hge = hgeCreate( HGE_VERSION );
    LoadWorld();
}

World::~World()
{
    // clear world contents
    for ( WorldObjectList::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
    {
        delete ( *i );
    }
    m_objects.clear();

    m_hge->Release();
}

void World::LoadWorld()
{
    int screenHeight = m_hge->System_GetState( HGE_SCREENHEIGHT );
    m_targetResToActualResScale = static_cast<float>( screenHeight ) / static_cast<float>( TARGET_SCREEN_HEIGHT );
    m_cellBoxHeight = screenHeight / m_worldHeight;
    m_cellBoxSize = m_cellBoxHeight * m_cellBoxHeight;

    m_worldCellObjects.resize( m_worldHeight * m_worldWidth );

    // TODO: It feels wasteful but I think an array of these is fine rather than a list
    // though I want to be able to remove nonvisible sections at a later point easily.
    // Maybe I can implement using 2 maze generations at once, and essentially have 2 mazes
    // on top of each other, so that I can swap between one and the other during nonvisible
    // retreads.
    for ( int row = 0; row < m_worldHeight; ++row )
    {
        for ( int col = 0; col < m_worldWidth; ++col )
        {
            m_worldCellObjects[row * m_worldWidth + col] = WorldObject( this, static_cast<float>(col * m_cellBoxHeight), static_cast<float>(row * m_cellBoxHeight) );
            if ( row <= 1 || row >= m_worldHeight - 2 ||
                 col <= 1 || col >= m_worldWidth - 2 )
            {
                m_worldCells[row][col] = WORLD_CELL_WALL1;
            }
            else
            {
                m_worldCells[row][col] = WORLD_CELL_EMPTY;
            }
        }
    }

    // TODO: remove this test cell initialization Just need to draw real cells
    // I think here we'd want to instead actually generate the maze so we can create the
    // list of WorldObjects to draw them.
    m_worldCells[4][11] = WORLD_CELL_WALL1;
    m_worldCells[2][22] = WORLD_CELL_WALL1;
    m_worldCells[11][33] = WORLD_CELL_WALL1;
    m_worldCells[19][2] = WORLD_CELL_WALL1;

    for ( int row = 0; row < m_worldHeight; ++row )
    {
        for ( int col = 0; col < m_worldWidth; ++col )
        {
            m_worldCellObjects[row * m_worldWidth + col].m_celltype = m_worldCells[row][col];
        }
    }

    m_fog.Init( this );

    m_worldLoaded = true;
}

void World::Think()
{
    if ( m_pauseFlag )
        return;
}

void World::OnPlayerDied()
{
    // play some animations and maybe restart the game
    m_pauseFlag = true;
}

CellType &World::At( uint32_t row, uint32_t col )
{
    _ASSERTE( row >= 0 && row < m_worldHeight );
    _ASSERTE( col >= 0 && col < m_worldWidth );
    return m_worldCells[row][col];
}

WorldObject &World::WorldObjectAt( float y, float x )
{
    //_ASSERTE( row >= 0 && row < m_worldHeight );
    //_ASSERTE( col >= 0 && col < m_worldWidth );

    uint32_t row = static_cast<uint32_t>( y / m_cellBoxHeight );
    uint32_t col = static_cast<uint32_t>( x / m_cellBoxHeight );
    return m_worldCellObjects[row * m_worldWidth + col];
}

void World::Render()
{
    m_hge->Gfx_Clear( hgeColor32( 255, 0, 0, 0 ) );

    // draw our terrain aka our maze

    // feels very wasteful to pass a big array where cells are empty
    // I should compare versus array of ptrs of stucts
    RenderWorldCells( this );

    m_fog.Render();
}

SquareCollisionResult World::TestBlockCollisionAt( const hgeRect &rc )
{
    // we simplify calculation by only testing 4 corners
    // using as advantage the fact, that player has same size as world blocks
    hgeVector collisionDist(0.0f, 0.0f);
    if ( IsSolidAtXY( rc.x1, rc.y1 ) )
    {
        hgeRect collidedBox = WorldObjectAt( rc.y1, rc.x1 ).m_pos;
        collisionDist.x = std::max<float>( -1.0f * abs( rc.x1 - collidedBox.x1 ), -1.0f * abs( rc.x1 - collidedBox.x2 ) ) - 1.0f;
        collisionDist.y = std::max<float>( -1.0f * abs( rc.y1 - collidedBox.y1 ), -1.0f * abs( rc.y1 - collidedBox.y2 ) ) - 1.0f;

        return { false, collisionDist };
    }
    if ( IsSolidAtXY( rc.x1, rc.y2 ) )
    {
        hgeRect collidedBox = WorldObjectAt( rc.y2, rc.x1 ).m_pos;
        collisionDist.x = std::max<float>( -1.0f * abs( rc.x1 - collidedBox.x1 ), -1.0f * abs( rc.x1 - collidedBox.x2 ) ) - 1.0f;
        collisionDist.y = std::max<float>( -1.0f * abs( rc.y2 - collidedBox.y1 ), -1.0f * abs( rc.y2 - collidedBox.y2 ) ) - 1.0f;

        return { false, collisionDist };
    }
    if ( IsSolidAtXY( rc.x2, rc.y1 ) )
    {
        hgeRect collidedBox = WorldObjectAt( rc.y1, rc.x2 ).m_pos;
        collisionDist.x = std::max<float>( -1.0f * abs( rc.x2 - collidedBox.x2 ), -1.0f * abs( rc.x2 - collidedBox.x1 ) ) - 1.0f;
        collisionDist.y = std::max<float>( -1.0f * abs( rc.y1 - collidedBox.y1 ), -1.0f * abs( rc.y1 - collidedBox.y2 ) ) - 1.0f;
        return { false, collisionDist };
    }
    if ( IsSolidAtXY( rc.x2, rc.y2 ) )
    {
        hgeRect collidedBox = WorldObjectAt( rc.y2, rc.x2 ).m_pos;
        collisionDist.x = std::max<float>( -1.0f * abs( rc.x2 - collidedBox.x2 ), -1.0f * abs( rc.x2 - collidedBox.x1 ) ) - 1.0f;
        collisionDist.y = std::max<float>( -1.0f * abs( rc.y2 - collidedBox.y1 ), -1.0f * abs( rc.y2 - collidedBox.y2 ) ) - 1.0f;
        return { false, collisionDist };
    }



    return { true, collisionDist };
}

bool World::IsSolid( CellType contents )
{
    return contents == WORLD_CELL_WALL1;
}

bool World::IsKillOnTouch( CellType contents )
{
    return contents == WORLD_CELL_SPIKES;
}

void World::FindIntersectingObjects( const hgeRect &rc, WorldObjectList &result )
{
    result.clear();
    // for ( WorldObjectList::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
    //{
    //     if ( ( *i )->m_box.Intersect( &rc ) )
    //     {
    //         result.push_back( *i );
    //     }
    // }
}

void World::RemoveObject( WorldObject *o )
{
    // TODO: When object list is large enough replace with std::unordered_map or some type of hash table impl
    for ( WorldObjectList::iterator i = m_objects.begin(); i != m_objects.end(); ++i )
    {
        if ( *i == o )
        {
            m_objects.erase( i );
            return;
        }
    }
}


void World::GoalReached()
{
    m_levelGoalReached = true;
}

bool World::Victory()
{
    return m_levelGoalReached;
}

// Player class
// controls keyboard interaction and game rules
#include "Player.h"
#include "Debug.h"
#include "Game.h"
#include "World.h"
#include "WorldObject.h"

#undef min

#include <algorithm>

const static hgeVector g_defaultPlayerStart = hgeVector( 300, 300 );

Player::Player()
    : m_playerBox(), m_position( 0, 0, 0, 0 ), m_speed( 0, 0 ), 
       m_world( nullptr ), m_is_dead( false ), m_lives( INITIAL_LIVES_COUNT )
{
    m_game = MyGame::GetGame();
    // this will not create another HGE, instead we will get the global
    // unique HGE object which is already started
    m_hge = hgeCreate( HGE_VERSION );

    m_playerBox.blend = BLEND_DEFAULT_Z;
}


Player::~Player()
{
}


void Player::EnterWorld( World *world )
{
    _ASSERTE( world );
    _ASSERTE( world->IsWorldLoaded() );

    m_world = world;

    hgeVector player_start = g_defaultPlayerStart;
    MoveTo( player_start.x, player_start.y );
}


hgeRect Player::GetScreenPosition()
{
    hgeRect spos = m_position;
    return spos;
}


void Player::HandleMovementAndCollision( float delta )
{


    if ( m_hge->Input_GetKeyState( HGEK_RIGHT ) || m_hge->Input_GetKeyState( HGEK_D ) )
    {
        m_speed.x = 1;
    }
    else if ( m_hge->Input_GetKeyState( HGEK_LEFT ) || m_hge->Input_GetKeyState( HGEK_A ) )
    {
        m_speed.x = -1;
    }
    else
    {
        m_speed.x = 0;
    }

    if ( m_hge->Input_GetKeyState( HGEK_UP ) || m_hge->Input_GetKeyState( HGEK_W ) )
    {
        m_speed.y = -1;
    }
    else if ( m_hge->Input_GetKeyState( HGEK_DOWN ) || m_hge->Input_GetKeyState( HGEK_S ) )
    {
        m_speed.y = 1;
    }
    else
    {
        m_speed.y = 0;
    }

    m_speed.Normalize();
    m_speed *= BASE_MOVING_SPEED * m_world->m_targetResToActualResScale;

    float dy = m_speed.y * delta;
    float dx = m_speed.x * delta;

    hgeRect new_pos = m_position;
    if ( m_speed.x != 0 )
    {
        hgeRect future_pos = m_position;
        future_pos.x1 += dx;
        future_pos.x2 += dx;

        SquareCollisionResult xResult = m_world->TestBlockCollisionAt( future_pos );
        hgeVector collisionDelta = xResult.second;

        float correctedDx;
        if ( dx < 0 )
        {
            correctedDx = ( dx - collisionDelta.x );
        }
        else
        {
            correctedDx = ( dx + collisionDelta.x );
        }

        new_pos.x1 += correctedDx;
        new_pos.x2 += correctedDx;
    }
    if ( m_speed.y != 0 )
    {
        hgeRect future_pos = m_position;
        future_pos.y1 += dy;
        future_pos.y2 += dy;

        SquareCollisionResult yResult = m_world->TestBlockCollisionAt( future_pos );
        hgeVector collisionDelta = yResult.second;
        float correctedDy;
        if ( dy < 0 )
        {
            correctedDy = ( dy - collisionDelta.y );
        }
        else
        {
            correctedDy = ( dy + collisionDelta.y );
        }
        new_pos.y1 += correctedDy;
        new_pos.y2 += correctedDy;
    }


    m_position = new_pos;
#ifdef _DEBUG
    GetGlobalDebugInfo()->playerPos = m_position;
#endif
}


void Player::Think()
{
    if ( m_is_dead )
    {
        // press ENTER when dead leads to respawn
        if ( m_hge->Input_GetKeyState( HGEK_ENTER ) )
        {
            Respawn();
        }
        return;
    }
    if ( m_world->m_pauseFlag )
    {
        // check for pause key to unpause
        return;
    }

    // float delta = m_hge->Timer_GetDelta();
    float delta = m_game->accumulatedDeltaTime;

    HandleMovementAndCollision( delta );

    WorldObjectList touching_objects;

    m_world->FindIntersectingObjects( m_position, touching_objects );
}


void Player::Render( World *world )
{
    _ASSERTE( world );
    _ASSERTE( m_game );
    MyGame *game = m_game;

    int screenWidth = m_hge->System_GetState( HGE_SCREENWIDTH );

    // TODO: Move this font print and logic outside of player class. 
    // print with white shadow
    game->m_font->SetScale( m_world->m_targetResToActualResScale );
    game->m_font->SetBlendMode( BLEND_DEFAULT_Z );
    game->m_font->SetColor( hgeColor32( 255, 255, 255, 255 ) );
    game->m_font->printfb( 0, 0, screenWidth, 40, HGETEXT_RIGHT, "Lives: %d ", m_lives );
    game->m_font->SetColor( hgeColor32( 255, 0, 0, 0 ) );
    game->m_font->printfb( 2, 2, screenWidth, 40, HGETEXT_RIGHT, "Lives: %d ", m_lives );


    hgeRect pos = GetScreenPosition();

    // TODO: This should be a function so that in the future this can have a texture or something.

    hgeColor32 blue( 0xFF0000FF );

    hgeVertex p1 = { pos.x1, pos.y1, m_zLayer, blue.argb, 0.0f, 0.0f };
    hgeVertex p2 = { pos.x2, pos.y1, m_zLayer, blue.argb, 0.0f, 0.0f };
    hgeVertex p3 = { pos.x2, pos.y2, m_zLayer, blue.argb, 0.0f, 0.0f };
    hgeVertex p4 = { pos.x1, pos.y2, m_zLayer, blue.argb, 0.0f, 0.0f };

    // m_playerBox = { { p1, p2, p3, p4 }, 0, BLEND_DEFAULT_Z };
    m_playerBox.v[0] = p1;
    m_playerBox.v[1] = p2;
    m_playerBox.v[2] = p3;
    m_playerBox.v[3] = p4;

    // m_hge->Gfx_SetShader( m_shader );
    m_hge->Gfx_RenderQuad( &m_playerBox );
    // m_hge->Gfx_SetShader( NULL );


    MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x1, pos.y1, pos.x2, pos.y1, hgeColor32::WHITE().argb, m_zLayer );
    MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x2, pos.y1, pos.x2, pos.y2, hgeColor32::WHITE().argb, m_zLayer );
    MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x2, pos.y2, pos.x1, pos.y2, hgeColor32::WHITE().argb, m_zLayer );
    MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x1, pos.y1, pos.x1, pos.y2, hgeColor32::WHITE().argb, m_zLayer );

}


void Player::MoveTo( float x, float y )
{
    _ASSERTE( m_world );
    m_position.Set(
        x, y,
        x + m_world->m_cellBoxHeight - 0.0f,
        y + m_world->m_cellBoxHeight - 0.0f );
}


void Player::Die()
{
    // can't die twice in a moment
    if ( m_is_dead )
        return;

    m_is_dead = true;
    m_world->OnPlayerDied();
}


void Player::Respawn()
{
    m_lives--;
    if ( m_lives >= 0 )
    {

        m_is_dead = false;
        m_world->m_pauseFlag = false;

        EnterWorld( m_world );
    }
}

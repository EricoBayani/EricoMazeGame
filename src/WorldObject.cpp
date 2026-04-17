#include "WorldObject.h"
#include "Debug.h"
#include "Game.h"
#include "Player.h"
#include "World.h"
WorldObject::WorldObject( World *world, float x, float y )
    : m_celltype( WORLD_CELL_EMPTY )
{
    // m_pos.Set( x, y, x + world->m_cellBoxHeight - 1.0f, y + world->m_cellBoxHeight - 1.0f );
    m_pos.Set( x, y, x + world->m_cellBoxHeight, y + world->m_cellBoxHeight );
}


void WorldFog::Init( World *world )
{
    float rescale = world->m_targetResToActualResScale;
    float rescaleX = static_cast<float>( world->TARGET_SCREEN_WIDTH ) * rescale;
    float rescaleY = static_cast<float>( world->TARGET_SCREEN_HEIGHT ) * rescale;

    m_pos.Set( 0, 0, rescaleX, rescaleY );

    hgeColor32 faded( 0x8F000000 );

    hgeVertex p1 = { m_pos.x1, m_pos.y1, m_zLayer, faded.argb, 0.0f, 0.0f };
    hgeVertex p2 = { m_pos.x2, m_pos.y1, m_zLayer, faded.argb, 0.0f, 0.0f };
    hgeVertex p3 = { m_pos.x2, m_pos.y2, m_zLayer, faded.argb, 0.0f, 0.0f };
    hgeVertex p4 = { m_pos.x1, m_pos.y2, m_zLayer, faded.argb, 0.0f, 0.0f };

    m_quad.v[0] = p1;
    m_quad.v[1] = p2;
    m_quad.v[2] = p3;
    m_quad.v[3] = p4;

    m_quad.blend = BLEND_DEFAULT_Z;

#ifdef _DEBUG

    GetGlobalDebugInfo()->fog = this;

#endif
}

void WorldFog::ResetFog()
{
    m_enabled = true;
}

void WorldFog::ToggleFog()
{
    m_enabled ^= m_enabled;
}

void WorldFog::Render()
{
    if ( !m_enabled )
    {
        return;
    }

    // set shader

    // render quad
    MyGame::GetGame()->m_hge->Gfx_RenderQuad( &m_quad );

    // unset shader
}


static void RenderWorldCell( const WorldObject &cell )
{

    switch ( cell.m_celltype )
    {

    case WORLD_CELL_WALL1 :
    {
        hgeRect pos = cell.m_pos;


        // TODO: This should be a function so that in the future this can have a texture or something.
        // also we need to do something with the cell type

        hgeColor32 offWhite( 0xFFCCCCCC );


        hgeVertex p1 = { pos.x1, pos.y1, 1.0f, offWhite.argb, 0.0f, 0.0f };
        hgeVertex p2 = { pos.x2, pos.y1, 1.0f, offWhite.argb, 0.0f, 0.0f };
        hgeVertex p3 = { pos.x2, pos.y2, 1.0f, offWhite.argb, 0.0f, 0.0f };
        hgeVertex p4 = { pos.x1, pos.y2, 1.0f, offWhite.argb, 0.0f, 0.0f };

        hgeQuad quad = { { p1, p2, p3, p4 }, 0, BLEND_DEFAULT_Z };

        MyGame::GetGame()->m_hge->Gfx_RenderQuad( &quad );

        // This is a debug thing I should be able to turn this off in release builds

        // hgeColor32 red( 0xFFFF0000 );
        hgeColor32 green( 0xFF00FF00 );
        hgeColor32 borderColor = green;

        MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x1, pos.y1, pos.x2, pos.y1, borderColor.argb, 1.0f );
        MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x2, pos.y1, pos.x2, pos.y2, borderColor.argb, 1.0f );
        MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x2, pos.y2, pos.x1, pos.y2, borderColor.argb, 1.0f );
        MyGame::GetGame()->m_hge->Gfx_RenderLine( pos.x1, pos.y1, pos.x1, pos.y2, borderColor.argb, 1.0f );
        break;
    }
    default :
    {
        break;
    }
    }
}

void RenderWorldCells( World *world )
{
    const auto &cells = world->GetWorldCellObjects();
    for ( int i = 0; i < cells.size(); ++i )
    {
        RenderWorldCell( cells[i] );
    }
}

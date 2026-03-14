#include "WorldObject.h"
#include "Game.h"
#include "Player.h"
#include "World.h"

WorldObject::WorldObject( World *world, float x, float y )
{
    // pos.Set( x, y, x + world->m_cellBoxHeight - 1.0f, y + world->m_cellBoxHeight - 1.0f );
    pos.Set( x, y, x + world->m_cellBoxHeight, y + world->m_cellBoxHeight );
}

static void RenderWorldCell( const WorldObject &cell )
{

    switch ( cell.celltype )
    {

    case WORLD_CELL_WALL1 :
    {
        hgeRect pos = cell.pos;


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

void RenderWorldCells( const std::vector<WorldObject> &cells )
{

    for ( int i = 0; i < cells.size(); ++i )
    {
        RenderWorldCell( cells[i] );
    }
}

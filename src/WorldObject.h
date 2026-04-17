#pragma once

#include <hgerect.h>
#include <hgesprite.h>

class World;
class Player;

typedef uint32_t CellType;

enum ECellType : CellType
{
    WORLD_CELL_EMPTY = ' ',
    WORLD_CELL_PLAYER_START = '@',
    WORLD_CELL_WALL1 = '#',
    WORLD_CELL_SPIKES = '^',
    WORLD_CELL_GOAL = '!',
    WORLD_CELL_ENEMY1 = 'A'
};


struct WorldObject
{
    hgeRect m_pos;       // I'm adopting top left corner as the position point. (x1, y1) = top-left (x2, y2) = bottom-right
    CellType m_celltype; // This is the enum value, more like a character really.
    //int worldX;
    //int worldY; // I think it'd be good to have cell's know where they are in the world

    WorldObject( World *world, float x, float y );
    WorldObject() = default;
}; 

struct WorldFog
{
    hgeQuad m_quad;
    hgeRect m_pos;
    HSHADER m_shader;
    HTEXTURE m_tex;

    bool m_enabled = true;

    const float m_zLayer = 0.4f; 

    void Init(World *world);

    void ResetFog();
    void ToggleFog();

    void Render();
};

void RenderWorldCells( World *world );
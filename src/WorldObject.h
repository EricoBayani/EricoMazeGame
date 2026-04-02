#pragma once

#include <hgerect.h>
#include <hgesprite.h>

#include "World.h"

class World;
class Player;


struct WorldObject
{
    hgeRect pos;       // I'm adopting top left corner as the position point. (x1, y1) = top-left (x2, y2) = bottom-right
    CellType celltype; // This is the enum value, more like a character really.
    int worldX;
    int worldY; // I think it'd be good to have cell's know where they are in the world

    WorldObject( World *world, float x, float y );
    WorldObject() = default;
}; 

void RenderWorldCells( const std::vector<WorldObject> &cells );
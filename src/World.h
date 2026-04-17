#pragma once

#include <list>
#include <string>
#include <vector>

#include <hge.h>
#include <hgerect.h>
#include <hgevector.h>

#include <cstdint>
#include <crtdbg.h>

#include "WorldObject.h"

class Player;

class hgeSprite;

typedef std::vector<WorldObject *> WorldObjectList;
typedef std::pair<bool, hgeVector> SquareCollisionResult;

class World
{
  protected:
    WorldFog m_fog;

    Player *m_player;


    bool m_worldLoaded = false;

    int m_worldWidth = 40;
    int m_worldHeight = 30;

    std::vector<WorldObject> m_worldCellObjects;

    CellType m_worldCells[30][40] = { WORLD_CELL_EMPTY };


    WorldObjectList m_objects;

  public:
    bool m_pauseFlag;
    bool m_levelGoalReached;

    HGE *m_hge;

    // pixel size of world cells to be determined by actual screen dimensions
    int m_cellBoxSize;

    int m_cellBoxHeight;

    static const int TARGET_SCREEN_WIDTH = 800;
    static const int TARGET_SCREEN_HEIGHT = 600;

    float m_targetResToActualResScale = 1.0;

  public:
    World( Player *plr );
    World() = default;

    virtual ~World();

    virtual bool Victory();

    virtual void Think();

    virtual void LoadWorld();

    virtual bool IsWorldLoaded() { return m_worldLoaded; };

    virtual void Render();

    CellType &At( uint32_t row, uint32_t col );


    WorldObject &World::WorldObjectAt( float y, float x );

    inline CellType AtXY( float x, float y )
    {
        return At( static_cast<uint32_t>( y / m_cellBoxHeight ), static_cast<uint32_t>( x / m_cellBoxHeight ) );
    }

    // tests if rect rc is allowed to be in the world and does not collide a solid block
    virtual SquareCollisionResult TestBlockCollisionAt( const hgeRect &rc );

    // tests if cell type is solid or pass-through using screenspace coord
    inline bool IsSolidAtXY( float x, float y )
    {
        return IsSolid(
            At( static_cast<uint32_t>( y / m_cellBoxHeight ), static_cast<uint32_t>( x / m_cellBoxHeight ) ) );
    }

    virtual bool IsSolid( CellType contents );

    virtual bool IsKillOnTouch( CellType contents );

    virtual void OnPlayerDied();

    // Scan world objects and find those whose bounding boxes touch the given box 'rc'
    // Result is placed in the 'result' list, can be 0 or 1 or multiple objects
    void FindIntersectingObjects( const hgeRect &rc, WorldObjectList &result );

    void RemoveObject( WorldObject *o );

    virtual void GoalReached();

    inline const std::vector<WorldObject>& GetWorldCellObjects()
    {
        return m_worldCellObjects;
    }
};

class WorldType1 : virtual public World
{
  public:
    bool Victory();
};

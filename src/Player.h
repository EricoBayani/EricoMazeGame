
#pragma once

#include <hge.h>
#include <hgerect.h>
#include <hgesprite.h>
#include <hgevector.h>

class World;

class MyGame;

class Player
{
  protected:
    MyGame *m_game = nullptr;
    hgeVector m_speed;

    // pixels per second when player is moving
    static const int BASE_MOVING_SPEED = 400;

    int m_lives;
    bool m_is_dead;

    World *m_world;
    HGE *m_hge;

  public:

    hgeRect m_position;
    const static int INITIAL_LIVES_COUNT = 3;

  public:
    Player();
    virtual ~Player();

    hgeRect GetScreenPosition();

    virtual void Die();

    virtual void Render( World *world );

    virtual void EnterWorld( World *world );

    virtual void Think();

    void MoveTo( float x, float y );

    virtual void Respawn();

    virtual void HandleMovementAndCollision( float delta );
};


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

    hgeQuad m_playerBox;
    hgeRect m_position;

    MyGame *m_game = nullptr;
    hgeVector m_speed;

    World *m_world;
    HGE *m_hge;

    int m_lives;
    bool m_is_dead;
  public:

    static constexpr float m_zLayer = 0.8f;

    static const int INITIAL_LIVES_COUNT = 3;
    // pixels per second when player is moving
    static const int BASE_MOVING_SPEED = 400;

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

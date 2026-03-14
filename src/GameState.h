#pragma once

#include <hgegui.h>

class MyGame;

class GameState
{
  protected:
    MyGame *m_game;
  public:
    virtual ~GameState() {}

    virtual void Render( ) = 0;

    virtual bool Think( ) = 0;

    virtual void OnEnterState( ) {}
};

class World;
class Player;

class GameState_Play : virtual public GameState
{

    World *m_world;
    Player *m_player;

  public:
    GameState_Play();
    virtual ~GameState_Play();

    virtual void Render();
    virtual bool Think( );
    void OnEnterState(  );
};

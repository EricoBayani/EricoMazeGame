
// GameState class
// Represents current state of the application - is it in menu screen, or in the
// game world, or at the game over screen (showing high scores screen).
#include "GameState.h"
#include "Game.h"
// #include "menuitem.h"
#include "Player.h"
#include "World.h"

#include <hgeguictrls.h>

#include <cmath>

void GameState_Play::Render()
{
    if ( m_world )
        m_world->Render();
    if ( m_player )
        m_player->Render( m_world );

    float mx, my;
    m_game->m_hge->Input_GetMousePos( &mx, &my );
}

bool GameState_Play::Think()
{
    m_world->Think();

    m_player->Think();

    if ( m_world->Victory() )
    {
        return false;
    }

    return false;
}

void GameState_Play::OnEnterState()
{
    delete m_player;
    delete m_world;
    m_player = new Player();
    m_world = new World( m_player );

    m_player->EnterWorld( m_world );
}

GameState_Play::GameState_Play()
    : m_world( nullptr ), m_player( nullptr )
{
    m_game = MyGame::GetGame();
}

GameState_Play::~GameState_Play()
{
    delete m_world;
    delete m_player;
}

#include "Game.h"
#include "GameState.h"

#include "Debug.h"

#include "imgui_internal.h"

#include <cstdio>

static MyGame *g_game = nullptr;

MyGame::MyGame()
    : m_font( nullptr )
{
    g_game = this;
}

MyGame *MyGame::GetGame()
{
    return g_game;
}

bool MyGame::FrameFunc()
{

    return GetGame()->m_state->Think();
}

bool MyGame::RenderFunc()
{

    GetGame()->m_hge->Gfx_BeginScene();

    GetGame()->m_state->Render();

    // flush the buffer before imgui draw
    int primsDrawn = 0;
    GetGame()->m_hge->Gfx_StartBatch( HGEPRIM_QUADS, 0, hgeBlendMode::BLEND_DEFAULT, &primsDrawn );

    RenderImGuiViews();

    GetGame()->m_hge->Gfx_EndScene();

    return false;
}

bool MyGame::Startup()
{

    m_hge = hgeCreate( HGE_VERSION );

    m_hge->System_SetState( HGE_SHOWSPLASH, false );
    m_hge->System_SetState( HGE_FPS, 60 );
     //m_hge->System_SetState( HGE_FPS, HGEFPS_VSYNC );
    // m_hge->System_SetState( HGE_FPS, HGEFPS_UNLIMITED );
    m_hge->System_SetState( HGE_LOGFILE, "MazeGame.log" );
    m_hge->System_SetState( HGE_FRAMEFUNC, FrameFunc );
    m_hge->System_SetState( HGE_RENDERFUNC, RenderFunc );
    m_hge->System_SetState( HGE_TITLE, "MazeGame" );
    m_hge->System_SetState( HGE_WINDOWED, true );
    // m_hge->System_SetState( HGE_SCREENWIDTH, 800 );
    // m_hge->System_SetState( HGE_SCREENHEIGHT, 600 );
    m_hge->System_SetState( HGE_SCREENWIDTH, 1600 );
    m_hge->System_SetState( HGE_SCREENHEIGHT, 1200 );
    m_hge->System_SetState( HGE_SCREENBPP, 32 );
    m_hge->System_SetState( HGE_HIDEMOUSE, false );
    m_hge->System_SetState( HGE_ZBUFFER, true );
    m_hge->System_SetState( HGE_USESOUND, false );


    if ( !m_hge->System_Initiate() )
        return false;


    m_font = new hgeFont( "font1.fnt" );


    SetupImGui();


    m_state_play = new GameState_Play();

    ShowPlayScreen();

    return true;
}

void MyGame::Shutdown()
{

    delete m_state_play;

    delete m_font;

    // Clean up and shutdown

    ShutdownImGui();


    m_hge->System_Shutdown();
    m_hge->Release();
}

void MyGame::ShowPlayScreen()
{
    m_state = m_state_play;
    m_state_play->OnEnterState();
}

int WINAPI
WinMain( HINSTANCE, HINSTANCE, LPSTR,
         int )
{
    MyGame game;
    if ( game.Startup() )
    {
        game.m_hge->System_Start();
        game.Shutdown();
    }
}

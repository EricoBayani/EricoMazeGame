
#pragma once

// this is for standard integer types like uint32_t (very useful)
#include <stdint.h>

#include <hge.h>
#include <hgefont.h>


#include <imgui.h>


class GameState;

class MyGame
{
  public:
    HGE *m_hge;
    hgeFont *m_font;
    GameState *m_state;
    GameState *m_state_play;

  public:
    MyGame();

    static bool FrameFunc();
    static bool RenderFunc();

    static MyGame *GetGame();

    bool Startup();

    void Shutdown();

    void ShowPlayScreen();
};

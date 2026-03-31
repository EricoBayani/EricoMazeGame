
#pragma once

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

    float accumulatedDeltaTime = 0.0f;
    const float fixedTargetDeltaTime = 0.016f;

  public:
    MyGame();

    static bool FrameFunc();
    static bool RenderFunc();

    static MyGame *GetGame();

    bool Startup();

    void Shutdown();

    void ShowPlayScreen();
};

#pragma once

#include "Game.h"


#ifdef _DEBUG

struct WorldFog;

// Fill this up before imgui prints it
struct DebugInfo
{
    hgeRect playerPos;
    WorldFog *fog;
    int frameDeltaSpikes = 0;
    bool fogEnabled = true;
};

static DebugInfo g_DebugInfo;

inline DebugInfo *GetGlobalDebugInfo()
{
    return &g_DebugInfo;
}

void SetupImGui();
void ShutdownImGui();
void RenderImGui();

#endif
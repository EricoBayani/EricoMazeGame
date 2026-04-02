#pragma once

#include "Game.h"


#ifdef _DEBUG


// Fill this up before imgui prints it
struct DebugInfo
{
    hgeRect playerPos;
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
#pragma once

#include "Game.h"

// Fill this up before imgui prints it
struct DebugInfo
{
    hgeRect playerPos;
    int frameDeltaSpikes;
};

static DebugInfo g_DebugInfo;

inline DebugInfo &GetGlobalDebugInfo()
{
    return g_DebugInfo;
}

void SetupImGui();
void ShutdownImGui();
void RenderImGuiViews();

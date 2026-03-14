#pragma once

#include <cstdio>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Debug.h"
#include "Game.h"

// TODO: Figure out how to make cmake define preprocessor stuff so that I can wrap this body of these functions around #define blocks


void SetupImGui()
{
#ifdef _DEBUG
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui_ImplWin32_Init( MyGame::GetGame()->m_hge->System_GetState( HGE_HWND ) );
    ImGui_ImplDX9_Init( MyGame::GetGame()->m_hge->GetGAPIDevice() );
    ImGui::StyleColorsDark();
#endif
}

void ShutdownImGui()
{
#ifdef _DEBUG

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif
}


void RenderImGuiViews()
{
#ifdef _DEBUG
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    const DebugInfo &debugInfo = GetGlobalDebugInfo();

    // Draw framerate and time since previous frame
    char debugTextBuffer[64];
    _snprintf( debugTextBuffer, sizeof( debugTextBuffer ) - 1,
               "dt:%.3f\nFPS:%d",
               MyGame::GetGame()->m_hge->Timer_GetDelta(), MyGame::GetGame()->m_hge->Timer_GetFPS() );
    ImGui::Begin( "Dev Stuff" );

    ImGui::Text( debugTextBuffer );

    ImGui::Separator();

    memset( debugTextBuffer, 0, sizeof( debugTextBuffer ) );

    _snprintf( debugTextBuffer, sizeof( debugTextBuffer ) - 1,
               "Player x: %.3f\nPlayer y: %.3f\n",
               debugInfo.playerPos.x1, debugInfo.playerPos.y1 );

    ImGui::Text( debugTextBuffer );

    ImGui::Text( "frameDeltaSpikes: %d", debugInfo.frameDeltaSpikes );

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
#endif
}

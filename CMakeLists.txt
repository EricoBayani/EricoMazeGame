cmake_minimum_required(VERSION 3.20)

project(MazeGame)

set(DIRECTX_SDK_DIR $ENV{DXSDK_DIR})

if (NOT DIRECTX_SDK_DIR)
    message( FATAL_ERROR "Building this project currently relies on a specific version of DirectX 9. Cmake will exit")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ CACHE PATH "Single Directory for all executables.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/ CACHE PATH "Single Directory for all executables.")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/linklib CACHE PATH "Single Directory for all static libraries.")

set(HGE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/include")

if(MSVC)
    set(USE_WIN32 "WIN32")
else()
    message( FATAL_ERROR "This project is only for Windows at the moment, Cmake will exit")
endif()

add_subdirectory(libs/zlib)
add_subdirectory(libs/HGE/helpers)
add_subdirectory(libs/HGE/core)
add_subdirectory(libs/imgui)

add_executable(MazeGame ${USE_WIN32} 
                                     src/Debug.h
                                     src/Debug.cpp
                                     src/Game.cpp 
                                     src/Game.h
                                     src/GameState.cpp 
                                     src/GameState.h
                                     src/Player.cpp 
                                     src/Player.h
                                     src/World.cpp 
                                     src/World.h
                                     src/WorldObject.cpp 
                                     src/WorldObject.h
                                     )

target_include_directories(MazeGame PUBLIC ${HGE_INCLUDE_DIR} 
                                           ${DIRECTX_SDK_DIR}/Include
                                           ${PROJECT_SOURCE_DIR}/libs/imgui
                                           ${PROJECT_SOURCE_DIR}/libs/imgui/backends
                                           )
target_link_libraries(MazeGame PRIVATE hge hgehelpers imgui)

if(MSVC)
    set_target_properties(MazeGame PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci")
    target_compile_options(hgehelpers PRIVATE /W4)
    target_compile_options(hge PRIVATE /W4)
    target_compile_options(imgui PRIVATE /W4)
    target_compile_options(MazeGame PRIVATE /W4)

endif()

cmake_minimum_required(VERSION 3.20)

add_library(imgui STATIC ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp 
                         ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.h
                         ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_internal.h 
                         ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
                         ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp 
                         ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
                         ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
                         ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_win32.cpp 
                         ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_win32.h
                         ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_dx9.cpp 
                         ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_dx9.h
                         )

target_include_directories(imgui PRIVATE ${PROJECT_SOURCE_DIR}/libs/imgui 
                                         ${PROJECT_SOURCE_DIR}/libs/imgui/backends 
                                         ${DIRECTX_SDK_DIR}/include
                                         )
# target_link_libraries(imgui PRIVATE ${DIRECTX_SDK_DIR}/Lib/x86/d3dx9.lib ${DIRECTX_SDK_DIR}/Lib/x86/d3d9.lib)
target_link_libraries(imgui PRIVATE ${DIRECTX_SDK_DIR}/Lib/x64/d3dx9.lib ${DIRECTX_SDK_DIR}/Lib/x64/d3d9.lib)
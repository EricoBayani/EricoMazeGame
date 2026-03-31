# project(HGECore)
cmake_minimum_required(VERSION 3.20)

set(HGECORE_SRC
        demo.cpp
        graphics.cpp
        ini.cpp
        input.cpp
        power.cpp
        random.cpp
        resource.cpp
        sound.cpp
        system.cpp
        timer.cpp
        hge_impl.h
        BASS/bass.h
        ${HGE_INCLUDE_DIR}/hge.h
        ${HGE_INCLUDE_DIR}/hgeanim.h
        ${HGE_INCLUDE_DIR}/hgecolor.h
        ${HGE_INCLUDE_DIR}/hgedistort.h
        ${HGE_INCLUDE_DIR}/hgefont.h
        ${HGE_INCLUDE_DIR}/hgegui.h
        ${HGE_INCLUDE_DIR}/hgeguictrls.h
        ${HGE_INCLUDE_DIR}/hgeparticle.h
        ${HGE_INCLUDE_DIR}/hgerect.h
        ${HGE_INCLUDE_DIR}/hgeresource.h
        ${HGE_INCLUDE_DIR}/hgesprite.h
        ${HGE_INCLUDE_DIR}/hgestrings.h
        ${HGE_INCLUDE_DIR}/hgevector.h
        ${HGE_INCLUDE_DIR}/impl/hgegapi.h
        ${HGE_INCLUDE_DIR}/impl/hgehandle.h
        ${HGE_INCLUDE_DIR}/impl/hgekeys.h
        ${HGE_INCLUDE_DIR}/impl/hgedefs.h
        ${HGE_INCLUDE_DIR}/impl/hgecolor.h
        ${DIRECTX_SDK_DIR}/include
        # ${PROJECT_SOURCE_DIR}/libs/src/zlib/contrib/minizip/unzip.h
)

#------ target (for creating HGE.DLL) -----
add_library(hge STATIC ${HGECORE_SRC})

target_include_directories(hge  PRIVATE ${HGE_INCLUDE_DIR} 
                                PRIVATE ${DIRECTX_SDK_DIR}/Include 
                                PRIVATE ${PROJECT_SOURCE_DIR}/libs/imgui 
                                PRIVATE ${PROJECT_SOURCE_DIR}/libs/imgui/backends
                                PRIVATE ${PROJECT_SOURCE_DIR}/libs/zlib
                                PRIVATE ${PROJECT_SOURCE_DIR}/libs/zlib/contrib/minizip)

#------ link directories listed ------


# target_link_libraries(hge winmm zlib ${DIRECTX_SDK_DIR}/Lib/x86/d3dx9.lib ${DIRECTX_SDK_DIR}/Lib/x86/d3d9.lib)
target_link_libraries(hge PRIVATE winmm zlib ${DIRECTX_SDK_DIR}/Lib/x64/d3dx9.lib ${DIRECTX_SDK_DIR}/Lib/x64/d3d9.lib)

add_dependencies(hge hgehelpers imgui zlib)

#if (MSVC)
#    SET_TARGET_PROPERTIES(hge PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libci")
#endif ()
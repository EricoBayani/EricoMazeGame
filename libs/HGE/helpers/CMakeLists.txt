# project(HGEHelpers)
cmake_minimum_required(VERSION 3.20)

#------ sources ------
set(HGEHELPERS_SRC
        hgeanim.cpp
        hgecolor.cpp
        hgedistort.cpp
        hgefont.cpp
        hgegui.cpp
        hgeguictrls.cpp
        hgeparticle.cpp
        hgepmanager.cpp
        hgerect.cpp
        hgeresource.cpp
        hgesprite.cpp
        hgestrings.cpp
        hgevector.cpp
        parser.cpp
        resources.cpp
        parser.h
        resources.h
        ${HGE_INCLUDE_DIR}/hgegui.h
        )

#------ target -----
add_library(hgehelpers STATIC ${HGEHELPERS_SRC})

target_include_directories(hgehelpers PRIVATE ${HGE_INCLUDE_DIR}
                                      PRIVATE ${DIRECTX_SDK_DIR}/Include
                                      )

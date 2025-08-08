cmake_minimum_required(VERSION 3.15)
project(reaper_sdk LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Expose SDK headers
add_library(reaper_sdk_headers INTERFACE)
target_include_directories(reaper_sdk_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk
    ${CMAKE_CURRENT_SOURCE_DIR}/reaper-plugins
)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/WDL")
    target_include_directories(reaper_sdk_headers INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/WDL
    )
endif()

# example_m3u plug-in (Windows only)
if(WIN32)
    add_library(example_m3u MODULE sdk/example_m3u/import_m3u.cpp)
    target_link_libraries(example_m3u PRIVATE reaper_sdk_headers)
    set_target_properties(example_m3u PROPERTIES PREFIX "")
endif()

# example_raw plug-in
add_library(example_raw MODULE
    sdk/example_raw/pcmsink_raw.cpp
    sdk/example_raw/pcmsrc_raw.cpp
)
target_link_libraries(example_raw PRIVATE reaper_sdk_headers)
set_target_properties(example_raw PROPERTIES PREFIX "")
if(WIN32)
    target_sources(example_raw PRIVATE sdk/example_raw/res.rc)
endif()

# reaper_csurf plug-in
add_library(reaper_csurf MODULE
    reaper-plugins/reaper_csurf/csurf_01X.cpp
    reaper-plugins/reaper_csurf/csurf_alphatrack.cpp
    reaper-plugins/reaper_csurf/csurf_babyhui.cpp
    reaper-plugins/reaper_csurf/csurf_bcf2000.cpp
    reaper-plugins/reaper_csurf/csurf_faderport.cpp
    reaper-plugins/reaper_csurf/csurf_faderport2.cpp
    reaper-plugins/reaper_csurf/csurf_main.cpp
    reaper-plugins/reaper_csurf/csurf_mcu.cpp
    reaper-plugins/reaper_csurf/csurf_osc.cpp
    reaper-plugins/reaper_csurf/csurf_tranzport.cpp
    reaper-plugins/reaper_csurf/csurf_www.cpp
    reaper-plugins/reaper_csurf/osc.cpp
    reaper-plugins/reaper_csurf/osc_message.cpp
)
target_link_libraries(reaper_csurf PRIVATE reaper_sdk_headers)
set_target_properties(reaper_csurf PROPERTIES PREFIX "")
if(WIN32)
    target_sources(reaper_csurf PRIVATE reaper-plugins/reaper_csurf/res.rc)
endif()

# reaper_mp3 plug-in
add_library(reaper_mp3 MODULE
    reaper-plugins/reaper_mp3/mp3dec.cpp
    reaper-plugins/reaper_mp3/mp3_index.cpp
    reaper-plugins/reaper_mp3/pcmsink_mp3lame.cpp
    reaper-plugins/reaper_mp3/pcmsrc_mp3dec.cpp
    reaper-plugins/reaper_mp3/mpglib/common.cpp
    reaper-plugins/reaper_mp3/mpglib/interface.cpp
    reaper-plugins/reaper_mp3/mpglib/layer2.cpp
    reaper-plugins/reaper_mp3/mpglib/layer3.cpp
    reaper-plugins/reaper_mp3/mpglib/StdAfx.cpp
    reaper-plugins/reaper_mp3/mpglib/tabinit.cpp
    reaper-plugins/reaper_mp3/mpglib/dct64_i386.cpp
    reaper-plugins/reaper_mp3/mpglib/decode_i386.cpp
)
target_link_libraries(reaper_mp3 PRIVATE reaper_sdk_headers)
set_target_properties(reaper_mp3 PROPERTIES PREFIX "")
if(WIN32)
    target_sources(reaper_mp3 PRIVATE reaper-plugins/reaper_mp3/res.rc)
endif()


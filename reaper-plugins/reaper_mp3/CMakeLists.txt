include(${PROJECT_SOURCE_DIR}/cmake/ReaperPlugin.cmake)

set(MP3_SOURCES
  mp3dec.cpp
  mp3_index.cpp
  pcmsrc_mp3dec.cpp
  pcmsink_mp3lame.cpp
  mpglib/StdAfx.cpp
  mpglib/common.cpp
  mpglib/dct64_i386.cpp
  mpglib/decode_i386.cpp
  mpglib/interface.cpp
  mpglib/layer2.cpp
  mpglib/layer3.cpp
  mpglib/tabinit.cpp
  ${PROJECT_SOURCE_DIR}/WDL/WDL/lameencdec.cpp
)

add_reaper_plugin(reaper_mp3 SOURCES ${MP3_SOURCES})

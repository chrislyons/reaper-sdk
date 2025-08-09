include(${PROJECT_SOURCE_DIR}/cmake/ReaperPlugin.cmake)

add_reaper_plugin(reaper_csurf SOURCES
  csurf_main.cpp
  csurf_01X.cpp
  csurf_alphatrack.cpp
  csurf_babyhui.cpp
  csurf_bcf2000.cpp
  csurf_faderport.cpp
  csurf_faderport2.cpp
  csurf_mcu.cpp
  csurf_osc.cpp
  csurf_tranzport.cpp
  csurf_www.cpp
  osc.cpp
  osc_message.cpp
)

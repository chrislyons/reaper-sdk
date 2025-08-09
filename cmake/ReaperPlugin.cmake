function(add_reaper_plugin TARGET)
  cmake_parse_arguments(RP "" "" "SOURCES" ${ARGN})
  add_library(${TARGET} MODULE ${RP_SOURCES})

  if(WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res.rc")
      target_sources(${TARGET} PRIVATE res.rc)
    endif()
    set_target_properties(${TARGET} PROPERTIES SUFFIX ".dll")
  elseif(APPLE)
    set_target_properties(${TARGET} PROPERTIES SUFFIX ".dylib")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res.rc")
      set(RC_MAC_DLG ${CMAKE_CURRENT_BINARY_DIR}/res.rc_mac_dlg)
      set(RC_MAC_MENU ${CMAKE_CURRENT_BINARY_DIR}/res.rc_mac_menu)
      add_custom_command(
        OUTPUT ${RC_MAC_DLG} ${RC_MAC_MENU}
        COMMAND ${PROJECT_SOURCE_DIR}/WDL/bin/swell_dlggen ${CMAKE_CURRENT_SOURCE_DIR}/res.rc ${RC_MAC_DLG}
        COMMAND ${PROJECT_SOURCE_DIR}/WDL/bin/swell_menu ${CMAKE_CURRENT_SOURCE_DIR}/res.rc ${RC_MAC_MENU}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res.rc
        COMMENT "Generating SWELL resources"
      )
      target_sources(${TARGET} PRIVATE ${RC_MAC_DLG} ${RC_MAC_MENU})
      set_source_files_properties(${RC_MAC_DLG} ${RC_MAC_MENU} PROPERTIES HEADER_FILE_ONLY TRUE)
    endif()
    target_link_libraries(${TARGET} "-framework Cocoa" "-framework CoreAudio" "-framework CoreMIDI")
  else()
    set_target_properties(${TARGET} PROPERTIES SUFFIX ".so")
  endif()

  set(REAPER_SDK_ROOT ${PROJECT_SOURCE_DIR})
  set(REAPER_PLUGINS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

  target_include_directories(${TARGET} PRIVATE
    ${REAPER_PLUGINS_ROOT}
    ${REAPER_SDK_ROOT}/sdk
    ${REAPER_SDK_ROOT}/WDL/WDL
    ${REAPER_SDK_ROOT}/WDL/swell
    ${CMAKE_CURRENT_BINARY_DIR}
  )

  target_compile_features(${TARGET} PRIVATE cxx_std_17)
  target_compile_definitions(${TARGET} PRIVATE NOMINMAX)
endfunction()

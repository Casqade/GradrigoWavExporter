cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

cmake_policy(SET CMP0091 NEW)


set(TARGET GradrigoLoader)
project(${TARGET} VERSION 1.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if (NOT ${BUILD_SHARED_LIBS})
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


add_library(${TARGET}
  src/GradrigoLoader.cpp
)


include(GNUInstallDirs)
include(GenerateExportHeader)
generate_export_header(${TARGET}
  EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/Gradrigo/Gradrigo_export.hpp
)

target_include_directories(${TARGET} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


install(TARGETS ${TARGET}
  EXPORT ${TARGET} DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/exports/Gradrigo/Gradrigo_export.hpp
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Gradrigo"
)

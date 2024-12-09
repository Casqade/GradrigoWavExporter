cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

cmake_policy(SET CMP0091 NEW)


set(TARGET GradrigoWavExporter)
project(${TARGET} VERSION 1.0 LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if (NOT ${BUILD_SHARED_LIBS})
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


add_executable(${TARGET}
  src/GradrigoWavExporter.cpp
)

add_subdirectory(deps/GradrigoLoader)

target_link_libraries(${TARGET} PRIVATE
  GradrigoLoader
)

include(GNUInstallDirs)
target_include_directories(${TARGET} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_link_options(${TARGET} PRIVATE
    /NOIMPLIB
    /NOEXP
  )
else()
  target_compile_options(${TARGET} PRIVATE -m64)
  target_link_options(${TARGET} PRIVATE -m64)

  if(NOT ${BUILD_SHARED_LIBS})
    target_link_libraries(${TARGET} PRIVATE
      -static
    )
  endif()

endif()

# CMakeList.txt : CMake project for AmbiScreen

set(
  UTILS_DIR
  utils
)

set(
  LIBS_DIR
  lib
)

file(GLOB_RECURSE UTILS_SRC
  "${UTILS_DIR}/*.cpp"
  "${UTILS_DIR}/*.hpp"
  "${UTILS_DIR}/*.h"
)

file(GLOB_RECURSE LIBS_SRC
  "${LIBS_DIR}/serial/*.cpp"
  "${LIBS_DIR}/serial/*.h"
)

set(
  AMBISCREEN_SRC
  main.cpp
  ScreenManager.cpp
  ScreenManager.h
)

add_executable(
  AmbiScreen
  ${AMBISCREEN_SRC}
  ${UTILS_SRC}
  ${LIBS_SRC}
)

target_include_directories(
  AmbiScreen PRIVATE
  ${UTILS_DIR}
  ${LIBS_DIR}
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../modules")

find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})

find_package(Xfixes REQUIRED)

message(STATUS "XFIXES_FOUND = ${XFIXES_FOUND}")
message(STATUS "XFIXES_INCLUDE_DIR = ${XFIXES_INCLUDE_DIR}")
message(STATUS "XFIXES_LIBRARY = ${XFIXES_LIBRARY}")

find_package(Xinerama REQUIRED)

message(STATUS "XINERAMA_FOUND = ${XINERAMA_FOUND}")
message(STATUS "XINERAMA_INCLUDE_DIR = ${XINERAMA_INCLUDE_DIR}")
message(STATUS "XINERAMA_LIBRARY = ${XINERAMA_LIBRARY}")

target_link_libraries(
    AmbiScreen PRIVATE
    screen_capture_lite_static
    ${X11_LIBRARIES}
    ${XFIXES_LIBRARY}
    ${XINERAMA_LIBRARY}
)

source_group("utils" FILES ${UTILS_SRC})
source_group("lib" FILES ${LIBS_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AmbiScreen PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests

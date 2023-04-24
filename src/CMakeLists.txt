# CMakeList.txt : CMake project for AmbiScreen

if (NOT "${Boost_INCLUDE_DIR_override}" STREQUAL "DEFAULT")
    set(Boost_INCLUDE_DIR "${Boost_INCLUDE_DIR_override}")
endif()

find_package(Boost 1.82.0 REQUIRED)

if(!Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

message(STATUS Boost_LIBRARY_DIRS="${Boost_LIBRARY_DIRS}")
message(STATUS Boost_INCLUDE_DIRS="${Boost_INCLUDE_DIRS}")

set(
  UTILS_DIR
  utils
)

file(GLOB_RECURSE UTILS_SRC
  "${UTILS_DIR}/*.cpp"
  "${UTILS_DIR}/*.hpp"
  "${UTILS_DIR}/*.h"
)

set(
  AMBISCREEN_SRC
  main.cpp
  ScreenManager.cpp
  ScreenManager.h
  FrameSection.hpp
)

add_executable(
  AmbiScreen
  ${AMBISCREEN_SRC}
  ${UTILS_SRC}
)

target_include_directories(
  AmbiScreen PRIVATE
  ${UTILS_DIR}
  ${Boost_INCLUDE_DIRS}
)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

target_link_libraries(
    AmbiScreen PRIVATE
    screen_capture_lite_static
    Dwmapi
    ${Boost_LIBRARIES}
)

source_group("utils" FILES ${UTILS_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AmbiScreen PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests

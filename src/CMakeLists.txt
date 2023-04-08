# CMakeList.txt : CMake project for AmbiScreen


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
)

add_executable(
  AmbiScreen
  ${AMBISCREEN_SRC}
  ${UTILS_SRC}
)

target_include_directories(
  AmbiScreen PRIVATE
  ${UTILS_DIR}
)

target_link_libraries(
  AmbiScreen PRIVATE
  screen_capture_lite_static
  Dwmapi
)

source_group("utils" FILES ${UTILS_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AmbiScreen PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests

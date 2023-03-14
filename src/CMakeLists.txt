# CMakeList.txt : CMake project for AmbiScreen


set(
  AMBISCREEN_SOURCES
  main.cpp
)

add_executable(
  AmbiScreen
  ${AMBISCREEN_SOURCES}
)

target_link_libraries(
  AmbiScreen PRIVATE
  screen_capture_lite_static
  Dwmapi
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AmbiScreen PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests

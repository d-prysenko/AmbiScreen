# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.19)

project ("AmbiScreen")

# add BOOTS_INCLUDEDIR to environment or specify it in configuration step
set(Boost_INCLUDE_DIR_override "DEFAULT" CACHE STRING "")

add_subdirectory(dependencies/ScreenCaptureLite)

add_subdirectory(src)


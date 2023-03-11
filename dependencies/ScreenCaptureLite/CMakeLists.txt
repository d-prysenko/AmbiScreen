include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLE OFF)
set(BUILD_CSHARP OFF)

FetchContent_Declare(
  ScreenCaptureLite
  GIT_REPOSITORY https://github.com/d-prysenko/screen_capture_lite.git
  GIT_TAG 53aaf1148afc47d15eda181351720b1278c5230a
)

FetchContent_MakeAvailable(ScreenCaptureLite)

unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY CACHE)
unset(CMAKE_LIBRARY_OUTPUT_DIRECTORY CACHE)
unset(CMAKE_PDB_OUTPUT_DIRECTORY CACHE)
unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY CACHE)

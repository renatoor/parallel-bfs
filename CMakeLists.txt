cmake_minimum_required(VERSION 3.1)
project(BFS)

set (BFS_VERSION_MAJOR 0)
set (BFS_VERSION_MINOR 1)
set (BFS_VERSION_PATCH 0)

find_package(OpenMP REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra ${OpenMP_CXX_FLAGS}")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(lib)
add_subdirectory(src)

cmake_minimum_required(VERSION 3.1)
project(parallel-bfs)

set (BFS_VERSION_MAJOR 0)
set (BFS_VERSION_MINOR 1)
set (BFS_VERSION_PATCH 0)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_subdirectory(src)

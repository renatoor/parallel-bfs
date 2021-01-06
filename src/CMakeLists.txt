add_executable(parallel-bfs main.cpp)

target_compile_features(parallel-bfs PRIVATE cxx_std_17)

target_link_libraries(parallel-bfs PRIVATE bfslib OpenMP::OpenMP_CXX)

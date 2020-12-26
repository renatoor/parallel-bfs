set(HEADER_LIST
    "${BFS_SOURCE_DIR}/include/BitSet.h"
    "${BFS_SOURCE_DIR}/include/BFS.h"
    )

add_library(bfslib
    BitSet.cpp
    BFS.cpp
    ${HEADER_LIST}
    )

include_directories("${CMAKE_SOURCE_DIR}/include")

target_compile_features(bfslib PUBLIC cxx_std_17)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

#pragma once

#include <vector>
#include "BitSet.h"

class BFS {
public:

    BFS(std::vector<int> vertices, std::vector<int>edges);

    ~BFS();

    void search(int source);

private:

    void sparseLayer();

    void denseLayer();

    std::vector<int> _parent;
    std::vector<int> _sparse;
    BitSet _dense;
};


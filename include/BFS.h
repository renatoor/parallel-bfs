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

	int _degree;

	std::vector<int> _vertices;
	std::vector<int> _edges;
    std::vector<int> _parent;
    std::vector<int> _sparse;

    BitSet _dense;
};


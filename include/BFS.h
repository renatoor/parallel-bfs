#pragma once

#include <vector>
#include "BitSet.h"

enum class Representation {
	Sparse,
	Dense
};

class BFS {
public:

    BFS(std::vector<int> &vertices, std::vector<int> &edges);

    ~BFS();

    void search(int source);

private:

    void sparseLayer();

    void denseLayer();

	void switchToDense();

	void switchToSparse();

	int _n;
	int _size;
	int _degree;
	int _maxThreads;

	std::vector<int> _vertices;
	std::vector<int> _edges;
    std::vector<int> _parent;

	std::vector<int> _prefixSum;

    std::vector<int> _sparse;
	std::vector<int> _nextSparseSize;
    std::vector<std::vector<int>> _nextSparse;

    BitSet _dense;
	BitSet _nextDense;

	Representation _representation;
};


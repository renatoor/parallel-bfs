#include "BFS.h"

BFS::BFS(std::vector<int> vertices, std::vector<int>edges) :
    _parent(vertices.size() - 1, -1),
    _sparse(vertices.size() - 1),
    _dense(vertices.size() - 1) {}

BFS::~BFS() {}

void BFS::search(int source) {
    _parent[source] = source;
    _sparse.push_back(source);

    while(!_sparse.empty() || !_dense.empty()) {
    }
}

void BFS::sparseLayer() {
}

void BFS::denseLayer() {
}


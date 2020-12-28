#include "BFS.h"

#include <thread>
#include <iostream>
#include <future>
#include <utility>

BFS::BFS(std::vector<int> vertices, std::vector<int>edges) :
	_degree(0),
	_vertices(vertices),
	_edges(edges),
	_parent(vertices.size() - 1, -1),
    _dense(vertices.size() - 1) {}

BFS::~BFS() {}

void BFS::search(int source) {
    _parent[source] = source;
    _sparse.push_back(source);

    while(!_sparse.empty() || !_dense.empty()) {
		sparseLayer();
    }

	for (size_t i = 0; i < _parent.size(); i++) {
		if (_parent[i] == -1) continue;
		std::cout << "vertex " << i << " parent " << _parent[i] << std::endl;
	}
}

void BFS::sparseLayer() {
	std::vector<std::future<std::pair<std::vector<int>, int>>> futures;
	std::vector<int> nextLayer;
	int nextLayerDegree = 0;

	for (auto &vertex : _sparse) {
		futures.push_back(std::async(std::launch::async, [&] (int vertex) -> std::pair<std::vector<int>, int> {
			int degree = 0;
			std::vector<int> layer;

			for (int i = _vertices[vertex]; i < _vertices[vertex + 1]; i++) {
				int neighbor = _edges[i];

				if (_parent[neighbor] == -1) {
					_parent[neighbor] = vertex;
					layer.push_back(neighbor);
					degree += _vertices[neighbor + 1] - _vertices[neighbor];
				}
			}

			return std::make_pair(layer, degree);
		}, vertex));
	}

	for (auto &f : futures) {
		auto [layer, degree] = f.get();

		nextLayer.insert(nextLayer.end(), layer.begin(), layer.end());
		nextLayerDegree += degree;
	}

	_sparse = nextLayer;
	_degree = nextLayerDegree;
}

void BFS::denseLayer() {
}


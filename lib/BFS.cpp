#include "BFS.h"

#include <thread>
#include <iostream>
#include <future>
#include <utility>
#include <bitset>
#include <algorithm>

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
		//sparseLayer();
		switchToDense();
		denseLayer();
		//switchToSparse();
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
	std::vector<std::future<std::pair<std::vector<int>, int>>> futures;
	BitSet nextLayer = BitSet(_vertices.size() - 1);
	int nextLayerDegree = 0, neighbor;
	
	for (size_t i = 0; i < _vertices.size(); i++) {
		if (_parent[i] != -1) 
			continue;
		
		for (int j = _vertices[i]; j < _vertices[i + 1]; j++) {
			neighbor = _edges[j];
			
			if (! _dense.contains(neighbor))
				continue;
			
			_parent[i] = neighbor;
			nextLayer.insert(i);

			//std::cout << "After Contains:"  << nextLayer.contains(neighbor) << std::endl;
			nextLayerDegree += _vertices[neighbor + 1] - _vertices[neighbor];
			break;
		}
	}

	_dense = nextLayer;

	for(auto i = 0; i < nextLayer.data().size(); i++){
		std::cout << nextLayer.data()[i] << std::endl;
	}
}

void BFS::switchToDense() {
	for (auto &vertex : _sparse) {
		_dense.insert(vertex);
	}

	_sparse.clear();
}

void BFS::switchToSparse() {
	std::vector<std::future<std::vector<int>>> futures;

	std::vector<uint64_t> data = _dense.data();

	for (size_t i = 0; i < data.size(); i += 8) {
		futures.push_back(std::async(std::launch::async, [&data] (size_t begin, size_t end) -> std::vector<int> {
			std::vector<int> layer;

			for (size_t i = begin; i < std::min(data.size(), end); i++) {
				uint64_t bits = data[i];

				while (bits != 0) {
					int bit = __builtin_ctzll(bits);
					int vertex = bit + i * 64;

					layer.push_back(vertex);

					bits &= ~(1 << bit);
				}
			}

			return layer;
		}, i, i + 8));
	}

	for (auto &f : futures) {
		std::vector<int> layer = f.get();

		_sparse.insert(_sparse.end(), layer.begin(), layer.end());
	}

	_dense.clear();
}


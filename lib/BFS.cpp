#include "BFS.h"

#include <omp.h>

BFS::BFS(std::vector<int> &vertices, std::vector<int> &edges) :
	_n(vertices.size() - 1),
	_size(0),
	_degree(0),
	_maxThreads(omp_get_max_threads()),
	_vertices(vertices),
	_edges(edges),
	_parent(_n, -1),
	_prefixSum(_maxThreads + 1),
	_nextSparseSize(_maxThreads),
	_nextSparse(_maxThreads),
	_dense(_n),
	_nextDense(_n)
{
	_sparse.reserve(_n);

	for (int i = 0; i < _maxThreads; i++) {
		_nextSparse[i].reserve(_n);
	}
}

BFS::~BFS() {}

void BFS::search(int source) {
    _parent[source] = source;
    _sparse[0] = source;
	_size = 1;
	_representation = Representation::Sparse;

    while (_size != 0) {
		if (_representation == Representation::Sparse) {
			if (_degree <= _n >> 5) {
				sparseLayer();
			}
			else {
				switchToDense();
				denseLayer();
			}
		}
		else {
			if ((_size > _n >> 5) || (_degree > _n >> 5)) {
				denseLayer();
			}
			else {
				switchToSparse();
				sparseLayer();
			}
		}
    }
}

void BFS::sparseLayer() {
	int nextLayerDegree = 0, tid = 0, vertex, neighbor, i, j;

	std::fill(_nextSparseSize.begin(), _nextSparseSize.end(), 0);

	if (_size == 1) {
		vertex = _sparse[0];

		#pragma omp parallel for private(tid, neighbor) reduction(+:nextLayerDegree) schedule(guided)
		for (i = _vertices[vertex]; i < _vertices[vertex + 1]; i++) {
			tid = omp_get_thread_num();

			neighbor = _edges[i];

			if (_parent[neighbor] == -1) {
				_parent[neighbor] = vertex;

				_nextSparse[tid][_nextSparseSize[tid]++] = neighbor;

				nextLayerDegree += _vertices[neighbor + 1] - _vertices[neighbor];
			}
		}
	}
	else {
		#pragma omp parallel for private(tid, vertex, neighbor, j) reduction(+:nextLayerDegree) schedule(guided)
		for (i = 0; i < _size; i++) {
			tid = omp_get_thread_num();

			vertex = _sparse[i];

			for (j = _vertices[vertex]; j < _vertices[vertex + 1]; j++) {
				neighbor = _edges[j];

				if (_parent[neighbor] == -1) {
					_parent[neighbor] = vertex;

					_nextSparse[tid][_nextSparseSize[tid]++] = neighbor;

					nextLayerDegree += _vertices[neighbor + 1] - _vertices[neighbor];
				}
			}
		}
	}

	_sparse.clear();

	_prefixSum[0] = 0;

	for (i = 0; i < _maxThreads; i++) {
		_prefixSum[i + 1] = _prefixSum[i] + _nextSparseSize[i];
	}

	#pragma omp parallel for private(tid, j)
	for (i = 0; i < _maxThreads; i++) {
		tid = omp_get_thread_num();

		for (j = 0; j < _nextSparseSize[tid]; j++) {
			_sparse[_prefixSum[tid] + j] = _nextSparse[tid][j];
		}
	}

	_size = _prefixSum[_maxThreads];
	_degree = nextLayerDegree;
}

void BFS::denseLayer() {
	int nextLayerSize = 0, nextLayerDegree = 0, i, j, neighbor;

	_nextDense.clear();
	
	#pragma omp parallel for private(j, neighbor) reduction(+:nextLayerSize, nextLayerDegree) schedule(dynamic, 512)
	for (i = 0; i < _n; i++) {
		if (_parent[i] != -1) {
			continue;
		}
		
		for (j = _vertices[i]; j < _vertices[i + 1]; j++) {
			neighbor = _edges[j];
			
			if (!_dense.contains(neighbor)) {
				continue;
			}

			_parent[i] = neighbor;

			_nextDense.insert(i);

			nextLayerDegree += _vertices[i + 1] - _vertices[i];
			nextLayerSize++;
			break;
		}
	}

	_dense = _nextDense;
	_degree = nextLayerDegree;
	_size = nextLayerSize;
}

void BFS::switchToDense() {
	for (int i = 0; i < _size; i++) {
		_dense.insert(_sparse[i]);
	}

	_sparse.clear();
	_representation = Representation::Dense;
}

void BFS::switchToSparse() {
	uint64_t bits;
	int tid, i, j, bit, vertex;

	std::fill(_nextSparseSize.begin(), _nextSparseSize.end(), 0);

	std::vector<uint64_t> data = _dense.data();

	#pragma omp parallel for private(tid, bits, bit, vertex)
	for (i = 0; i < (int) data.size(); i++) {
		tid = omp_get_thread_num();

		bits = data[i];

		while (bits != 0) {
			bit = __builtin_ctzll(bits);
			vertex = bit + i * 64;

			_nextSparse[tid][_nextSparseSize[tid]++] = vertex;

			bits &= ~(1 << bit);
		}
	}

	_dense.clear();

	_prefixSum[0] = 0;

	for (i = 0; i < _maxThreads; i++) {
		_prefixSum[i + 1] = _prefixSum[i] + _nextSparseSize[i];
	}

	#pragma omp parallel for private(tid, j)
	for (i = 0; i < _maxThreads; i++) {
		tid = omp_get_thread_num();

		for (j = 0; j < _nextSparseSize[tid]; j++) {
			_sparse[_prefixSum[tid] + j] = _nextSparse[tid][j];
		}
	}

	_size = _prefixSum[_maxThreads];
	_representation = Representation::Sparse;
}


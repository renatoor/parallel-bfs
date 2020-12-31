#include "BitSet.h"

#include <algorithm>
#include <iostream>

BitSet::BitSet(size_t size) : _data((size + 63) / 64), _size(0) {}

BitSet::~BitSet() {}

BitSet& BitSet::operator=(BitSet &other) {
	_data = other._data;
	_size = other._size;

	return *this;
}

void BitSet::insert(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1ULL << (value & _bucket_mask);

    _data[bucket] |= bit;
    _size++;
}

void BitSet::remove(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1ULL << (value & _bucket_mask);

	_data[bucket] &= ~bit;
	_size--;
}

bool BitSet::contains(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1ULL << (value & _bucket_mask);

    return (_data[bucket] & bit) != 0;
}

bool BitSet::empty() {
    return _size == 0;
}

void BitSet::clear() {
	std::fill(_data.begin(), _data.end(), 0);
	_size = 0;
}

const std::vector<uint64_t>& BitSet::data() {
    return _data;
}

BitSet BitSet::operator=(BitSet const &r) {
    if (&r != this) {
        std::cout << "TAMANHO: " << r._size << std::endl;
        _data = r._data;
        _size = r._size;//r._size;
    }
    return *this;
}
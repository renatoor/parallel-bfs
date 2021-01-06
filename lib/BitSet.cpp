#include "BitSet.h"

BitSet::BitSet(size_t size) : _data((size >> 6) + 1) {}

BitSet::~BitSet() {}

BitSet& BitSet::operator=(BitSet &other) {
	_data = other._data;

	return *this;
}

void BitSet::insert(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1ULL << (value & _bucket_mask);

    _data[bucket] |= bit;
}

bool BitSet::contains(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1ULL << (value & _bucket_mask);

    return (_data[bucket] & bit) != 0;
}

void BitSet::clear() {
	std::fill(_data.begin(), _data.end(), 0);
}

const std::vector<uint64_t>& BitSet::data() {
    return _data;
}


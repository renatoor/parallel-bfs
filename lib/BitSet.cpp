#include "BitSet.h"

BitSet::BitSet(size_t size) : _data((size + 63) / 64), _size(0) {}

BitSet::~BitSet() {}

void BitSet::insert(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1 << (value & _bucket_mask);

    _data[bucket] |= bit;
    _size++;
}

bool BitSet::contains(uint64_t value) {
    size_t bucket = value >> _bucket_bits;
    uint64_t bit = 1 << (value & _bucket_mask);

    return (_data[bucket] & bit) != 0;
}

bool BitSet::empty() {
    return _size > 0;
}

const std::vector<uint64_t>& BitSet::data() {
    return _data;
}


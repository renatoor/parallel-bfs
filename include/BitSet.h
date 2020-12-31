#pragma once

#include <vector>
#include <cstdint>

class BitSet {
public:

    BitSet(size_t size);

    ~BitSet();

    BitSet& operator=(BitSet &other);

    void insert(uint64_t value);

	void remove(uint64_t value);

    bool contains(uint64_t value);

    bool empty();

    void clear();

    const std::vector<uint64_t>& data();

private:

    std::vector<uint64_t> _data;

    size_t _size;

    const uint64_t _bucket_bits = 6;
    const uint64_t _bucket_size = 1 << 6;
    const uint64_t _bucket_mask = _bucket_size - 1;
};



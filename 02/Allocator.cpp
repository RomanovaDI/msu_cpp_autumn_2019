#include <iostream>
#include <cerrno>
#include <stdlib.h>
#include <cstring>
#include "Allocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) {
    sizeAll = maxSize % allign ? (maxSize / allign + 1) * allign : maxSize;
    pointerStart = new char[sizeAll];
}

LinearAllocator::~LinearAllocator() {
    if (pointerStart)
        delete[] pointerStart;
}

void* LinearAllocator::alloc(size_t size) {
    size_t allign = 8;
    size_t tmp_size = size % allign ? size + 1 : size;
    if (tmp_size > sizeAll - sizeUsed)
        throw std::runtime_error("not enough memory");
    void *tmp_return = pointerStart + sizeUsed;
    sizeUsed += tmp_size;
    return tmp_return;
}

void LinearAllocator::reset() {
    sizeUsed = 0;
}
#include <iostream>
#include <cerrno>
#include <stdlib.h>
#include <cstring>
#include "Timer.h"
#include "Allocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) {
    sizeAll = maxSize % allign ? (maxSize / allign + 1) * allign : maxSize;
    if (!(pointerStart = malloc(sizeAll))) {
        sizeAll = 0;
        std::cout << "malloc failed: " << std::strerror(errno) << '\n';
    }
}

LinearAllocator::~LinearAllocator() {
    if (pointerStart)
        free(pointerStart);
}

void* LinearAllocator::alloc(size_t size) {
    size_t allign = 8;
    size_t tmp_size = size % allign ? size + 1 : size;
    if (tmp_size > sizeAll - sizeUsed) {
        std::cout << "not enough memory" << '\n';
        return nullptr;
    }
    void *tmp_return = pointerStart + sizeUsed;
    sizeUsed += tmp_size;
    return tmp_return;
}

void LinearAllocator::reset() {
    sizeUsed = 0;
}
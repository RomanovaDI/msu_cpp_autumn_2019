#include <iostream>
#include <cerrno>
#include <stdlib.h>
#include <cstring>
#include "Timer.h"
#include "Allocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) {
    sizeAll = maxSize % allign ? (maxSize / allign + 1) * allign : maxSize;
    try {
        pointerStart = new char[sizeAll];
    }
    catch (std::bad_alloc& ba) {
        std::cerr << "error: " << ba.what() << '\n';
    }
}

LinearAllocator::~LinearAllocator() {
    if (pointerStart)
        delete pointerStart;
}

void* LinearAllocator::alloc(size_t size) {
    size_t allign = 8;
    size_t tmp_size = size % allign ? size + 1 : size;
    try {
        if (tmp_size > sizeAll - sizeUsed)
            throw std::runtime_error("not enough memory");
        void *tmp_return = pointerStart + sizeUsed;
        sizeUsed += tmp_size;
        return tmp_return;
    }
    catch (std::runtime_error& e) {
        std::cerr << "error: " << e.what() << '\n';
        return nullptr;
    }
}

void LinearAllocator::reset() {
    sizeUsed = 0;
}
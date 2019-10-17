#pragma once

class LinearAllocator {
private:
    void* pointerStart = nullptr;
    size_t sizeAll = 0;
    size_t sizeUsed = 0;
    size_t allign = 8;
public:
    LinearAllocator(size_t maxSize); // Size in bytes
    ~LinearAllocator();
    void* alloc(size_t size); // Size in bytes
    void reset();
};
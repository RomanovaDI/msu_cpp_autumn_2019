#include <iostream>
#include <cerrno>
#include <stdlib.h>
#include <cstring>
#include "Timer.h"
#include "Allocator.h"

int check_LinearAllocator() {
    Timer t;
    size_t number_of_int = 1024 * 1024 * 50;
    LinearAllocator allocator(number_of_int * sizeof(int));
    int* mass1 = (int*) allocator.alloc(number_of_int / 5 * sizeof(int));
    std::memset(mass1, 1, number_of_int / 5 * sizeof(int));
    int* mass2 = (int*) allocator.alloc(number_of_int / 5 * sizeof(int));
    std::memset(mass2, 1, number_of_int / 5 * sizeof(int));
    int* mass3 = (int*) allocator.alloc(number_of_int / 5 * sizeof(int));
    std::memset(mass3, 1, number_of_int / 5 * sizeof(int));
    int* mass4 = (int*) allocator.alloc(number_of_int / 5 * sizeof(int));
    std::memset(mass4, 1, number_of_int / 5 * sizeof(int));
    int* mass5 = (int*) allocator.alloc(number_of_int / 5 * sizeof(int));
    std::memset(mass5, 1, number_of_int / 5 * sizeof(int));
    return 0;
}

int checkLinearAllocatorErrors() {
    LinearAllocator allocator(500);
    allocator.alloc(600);
    allocator.alloc(501);
    return 0;
}

int check_Malloc() {
    Timer t;
    size_t number_of_int = 1024 * 1024 * 50;
    int* mass1 = (int*) malloc(number_of_int / 5 * sizeof(int));
    std::memset(mass1, 1, number_of_int / 5 * sizeof(int));
    int* mass2 = (int*) malloc(number_of_int / 5 * sizeof(int));
    std::memset(mass2, 1, number_of_int / 5 * sizeof(int));
    int* mass3 = (int*) malloc(number_of_int / 5 * sizeof(int));
    std::memset(mass3, 1, number_of_int / 5 * sizeof(int));
    int* mass4 = (int*) malloc(number_of_int / 5 * sizeof(int));
    std::memset(mass4, 1, number_of_int / 5 * sizeof(int));
    int* mass5 = (int*) malloc(number_of_int / 5 * sizeof(int));
    std::memset(mass5, 1, number_of_int / 5 * sizeof(int));
    free(mass2);
    free(mass1);
    return 0;
}

int main(void) {
    check_LinearAllocator();
    check_Malloc();
    checkLinearAllocatorErrors();
    return 0;
}
#include <chrono>
#include <iostream>
#include <algorithm>    // std::generate
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us = 
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};

int RandomNumber () { return (std::rand()%100); }

int main()
{
    Timer t;
    int size = 10000;
    std::srand ( unsigned ( std::time(0) ) );
    std::vector<int> myvector (size * size);
    std::generate (myvector.begin(), myvector.end(), RandomNumber);
    int sum = 0;
    for (int j = 0; j < size; j++)
        for (int i = 0; i < size; ++i)
            sum += myvector[i * size + j];
    return 0;
}
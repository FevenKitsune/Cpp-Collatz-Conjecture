// CppCollatzConjecture.cpp : Defines the entry point for the application.
//

#include "CppCollatzConjecture.h"
constexpr auto ITERATIONS = 1000;


struct return_data {
    //uint64_t highest_number;
    uint64_t iteration_count;
    uint64_t initial_value;
};

void exec(uint64_t start, uint64_t end, return_data& return_reference) {
    uint64_t iterations;
    uint64_t n;
    uint64_t initial_number;

    // Initialize value
    return_reference.iteration_count = 0;

    for (uint64_t i = start; i < end; i++) {
        iterations = 0;
        n = i;
        initial_number = n;
        while (n > 1) {
            if (n % 2 == 0) {
                iterations += 1;
                n /= 2;
            }
            else {
                iterations += 2;
                n = ((n * 3) + 1) / 2;
            }
        }
        if (iterations > return_reference.iteration_count) {
            //return_reference.highest_number = 1;
            return_reference.iteration_count = iterations;
            return_reference.initial_value = initial_number;
        }
    }
}

void normalize_iteration_count(uint64_t& _iterations, unsigned int thread_count) {
    if (_iterations % thread_count == 0) {
        // Values already normalized, no changes needed.
        printf("Iterations already normalized, no changes needed.\nBlock size: %u\n", _iterations / thread_count);
        return;
    }
    else {
        _iterations -= _iterations % thread_count;
        printf("Iterations normalized to %i\nBlock size: %u\n", _iterations, _iterations / thread_count);
        return;
    }
}

int main() {
    // TODO: Not compiling with MSVC. 
    // const unsigned int thread_count = std::thread::hardware_concurrency();
    const unsigned int thread_count = 8;
    std::thread threads[thread_count];
    return_data returns[thread_count];
    uint64_t _iterations = ITERATIONS;

    printf("%u threads detected.\n", thread_count);
    normalize_iteration_count(_iterations, thread_count);

    printf("Beginning computation...\n");
    auto t1 = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < thread_count; i++) {
        threads[i] = std::thread(
            exec, i * (_iterations / thread_count),
            (i * (_iterations / thread_count)) + (_iterations / thread_count),
            std::ref(returns[i]));
    }
    for (auto& thread : threads) {
        thread.join();
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    for (int i = 0; i < thread_count; i++) {
        printf("Thread %u highest: %llu converges in %llu iterations\n", i, returns[i].initial_value,
            returns[i].iteration_count);
    }
    printf("Computation duration: %fms\n", ms_double.count());
    return 0;
}
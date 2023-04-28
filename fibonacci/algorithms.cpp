/*
    Compilation:

    emcc -sALLOW_MEMORY_GROWTH algorithms.cpp -o algorithms.mjs -O3 -sASSERTIONS -s INITIAL_MEMORY=3GB -s MAXIMUM_MEMORY=3GB
*/

#include <cstdint>
#include <vector>
#include <emscripten.h>

#define EXTERN extern "C"

EXTERN EMSCRIPTEN_KEEPALIVE
uint64_t fibonacci(uint64_t n) {
    if (n == 0) return 0;
    else if (n == 1) return 1;
    uint64_t a = 0;
    uint64_t b = 1;
    for (uint64_t i = 2; i <= n; i++) {
        const uint64_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

EXTERN EMSCRIPTEN_KEEPALIVE
long* sieveOfEratosthenes(long limit) {
    std::vector<long> primes;

    bool* isPrime = new bool[limit + 1];
    for (long i = 0; i <= limit; i++) {
      isPrime[i] = true;
    }

    for (long p = 2; p * p <= limit; p++) {
      if (isPrime[p]) {
        for (long i = p * p; i <= limit; i += p) {
          isPrime[i] = false;
        }
      }
    }

    for (long p = 2; p <= limit; p++) {
      if (isPrime[p]) primes.push_back(p);
    }

    long* result = new long[primes.size()];
    std::copy(primes.begin(), primes.end(), result);
    delete[] isPrime;

    return result;
}

EXTERN EMSCRIPTEN_KEEPALIVE
double monteCarloPi(int times) {
    int inside = 0;
    for (int i = 0; i < times; i++) {
      double x = (double)std::rand() / RAND_MAX;
      double y = (double)std::rand() / RAND_MAX;
      if (x * x + y * y <= 1.0) inside++;
    }
    return 4.0 * (double)inside / times;
}

EXTERN EMSCRIPTEN_KEEPALIVE
uint32_t djb2Hash(const char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
/*
    Compilation:

    emcc fibonacci.cpp -o fibonacci.js
*/

#include <cstdint>
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
/*
    Compilation:

    emcc memory.cpp -o memory.js

    With malloc and free:
    emcc memory.cpp malloc.cpp -o memory.js

*/

#include <stdlib.h>
#include <emscripten.h>

#define EXTERN extern "C"


void *wasmfree(void *ptr);

void *wasmmalloc(size_t size);

EXTERN EMSCRIPTEN_KEEPALIVE
int sum(int *arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

EXTERN EMSCRIPTEN_KEEPALIVE
const char* hello() {
    return "Hello World!";
}

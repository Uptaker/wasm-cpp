/*
    Compilation:

    emcc encoded.cpp -o encoded.js
*/

#include <stdlib.h>
#include <emscripten.h>

#define EXTERN extern "C"

EXTERN EMSCRIPTEN_KEEPALIVE
void *wasmmalloc(size_t size) {
    return malloc(size);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void wasmfree(void *ptr) {
    free(ptr);
}

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
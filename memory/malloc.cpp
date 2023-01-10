/*
    Compilation:

    emcc malloc.cpp -o malloc.js
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
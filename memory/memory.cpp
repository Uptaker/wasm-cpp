/*
    Compilation:

    emcc memory.cpp -o memory.js
*/

#include <iostream>
#include <emscripten.h>

#define EXTERN extern "C"

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
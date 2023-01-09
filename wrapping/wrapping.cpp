/*
    Compilation:

    emcc wrapping.cpp -o wrapping.js -s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=[ccall]

    EMSCRIPTEN_KEEPALIVE macro tells compiler to not remove a function even if it appears unused
*/

#include <iostream>
#include <emscripten.h> // this will be recognized by emcc

#define EXTERN extern "C" // required due to name mangling in C++

EXTERN EMSCRIPTEN_KEEPALIVE
int multiply(int a, int b) {
    return a * b;
}


EXTERN EMSCRIPTEN_KEEPALIVE
int main(void) {
    std::cout << multiply(3, 5) << std::endl;
    return 0;
}
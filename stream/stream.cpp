/*
    Compilation:

    emcc stream.cpp -o stream.js
*/

#include <cmath>
#include <iostream>
#include <emscripten.h>

#define EXTERN extern "C"

EXTERN EMSCRIPTEN_KEEPALIVE
int dec2bin(long decimal) {
    long binary = 0;
    int remaining, i = 1;
    while(decimal != 0) {
        remaining = decimal % 2;
        decimal /= 2;
        binary += remaining * i;
        i *= 10;
    }
    return binary;
}

EXTERN EMSCRIPTEN_KEEPALIVE
int bin2dec(long binary) {
    long decimal = 0, remaining, i = 0;
    while(binary != 0) {
        remaining = binary % 10;
        binary /= 10;
        decimal += remaining * pow(2, i);
        i++;
    }
    return decimal;
}
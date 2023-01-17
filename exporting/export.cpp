#include <emscripten.h>
#include <stdlib.h>

#define EXTERN extern "C"

extern void logger(double progress);

extern unsigned int clientTime();

EXTERN EMSCRIPTEN_KEEPALIVE
unsigned char *randomString(int length) {
    unsigned char *str = (unsigned char*) malloc(length + 1);
    srand(clientTime());

    for (int i = 0; i < length; i++) {
        str[i] = rand() % (127 - 33) + 33;
        logger(i + 1 / length);
    }
    str[length] = 0;
    
    return str;
}

#include <emscripten.h>
#include <bg2-io.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE
void sayHello()
{
    printf("Hello World from C!\n");
}

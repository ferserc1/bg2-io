#include <emscripten.h>
#include <bg2-io.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

typedef struct StringT {
    int length;
    char * data;
} String;

EMSCRIPTEN_KEEPALIVE
void freeString(String * s, int debug) {
    debugLogParam("Releasing string", s->data, 20, debug);
    free(s->data);
    free(s);
}

EMSCRIPTEN_KEEPALIVE
Bg2File * loadBg2File(unsigned char * data, int size, int debug)
{
    debugLogFormat(debug, "Loading bg2 file...");

    Bg2File * file = NULL;
    Bg2ioBuffer * fileBuffer = (Bg2ioBuffer*)malloc(sizeof(Bg2ioBuffer));
    BG2IO_BUFFER_PTR_INIT(fileBuffer);

    debugLogFormat(debug, "Allocating memory (%s bytes)", size);

    // TODO: set fileBuffer->mem = data instead of copy the buffer
    if (bg2io_createBuffer(fileBuffer, size) < 0)
    {
        errorLog("Error creating file buffer. Could not allocate memory");
        free(fileBuffer);
        fileBuffer = NULL;
    }
    else
    {
        debugLog("Memory allocated", debug);
        debugLog("Copy buffer", debug);
        memcpy((void*)fileBuffer->mem, (const void*) data, size);
        fileBuffer->length = (int) size;
    }

    if (fileBuffer != NULL)
    {
        debugLog("Parsing bg2 file", debug);
        file = bg2io_parseFileBuffer(fileBuffer);
        if (file == NULL)
        {
            errorLog("Could not load bg2 file. Parse error");
        }

        bg2io_freeBuffer(fileBuffer);
        debugLog("Intermediate bg2 buffer released", debug);
    }

    return file;
}

EMSCRIPTEN_KEEPALIVE
Bg2FileHeader * getBg2FileHeader(Bg2File * file, int debug)
{
    return &file->header;
}

EMSCRIPTEN_KEEPALIVE
void freeBg2File(Bg2File * file, int debug)
{
    debugLog("Releasing bg2 file...", debug);
    bg2io_freeBg2File(file);
}

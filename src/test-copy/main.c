#include <stdlib.h>
#include <stdio.h>

#include "bg2-io.h"

Bg2ioBuffer * openFile(const char * file)
{
    Bg2ioBuffer * result = NULL;
    FILE * fd = fopen(file,"rb");
    if (fd)
    {
        fseek(fd, 0, SEEK_END);
        Bg2ioSize fileLength = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        result = (Bg2ioBuffer*) malloc(sizeof(Bg2ioBuffer));
        BG2IO_BUFFER_PTR_INIT(result);

        if (bg2io_createBuffer(result, fileLength) < 0)
        {
            printf("Error: could not allocate memory");
        }
        else
        {
            fread(result->mem, result->length, 1, fd);
            fclose(fd);
        }
    }
    else
    {
        printf("Error opening file at path %s", file);
    }
    return result;
}

int main(int argc, char ** argv)
{
    if (argc<2)
    {
        printf("usage: test-copy input.bg2\n");
        exit(1);
    }

    Bg2ioBuffer *fileBuffer = openFile(argv[1]);
    if (fileBuffer == NULL)
    {
        printf("Error opening bg2 file at path '%s'\n", argv[1]);
        exit(2);
    }

    Bg2File * fileData = bg2io_parseFileBuffer(fileBuffer);
    if (fileData == NULL)
    {
        printf("Malformed or invalid bg2 file. Error code: %d\n", bg2io_getParserError());
        exit(3);
    }

    Bg2ioBuffer outBuffer = BG2IO_BUFFER_INIT;
    int error = bg2io_writeFileToBuffer(fileData, &outBuffer);
    if (error != BG2IO_NO_ERROR)
    {
        printf("Error writing file to buffer. Error code %d", error);
    }


    bg2io_freeBuffer(fileBuffer);
    free(fileBuffer);
    bg2io_freeBuffer(&outBuffer);
    bg2io_freeBg2File(fileData);
    free(fileData);

    return 0;
}
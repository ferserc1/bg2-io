
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "bg2-io.h"

Bg2ioBuffer *openFile(const char * file)
{
    Bg2ioBuffer * result = NULL;
    FILE * fd;
    int errnum;

    fd = fopen(file, "rb");
    if (fd)
    {
        fseek(fd, 0, SEEK_END);
        Bg2ioSize fileLength = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        result = malloc(sizeof(Bg2ioBuffer));
        BG2IO_BUFFER_PTR_INIT(result);

        if (bg2io_createBuffer(result, fileLength) < 0)
        {
            printf("Error: could not lallocate memory");
        }
        else{
            fread(result->mem, result->length, 1, fd);

            fclose(fd);
        }
    }
    else
    {
        errnum = errno;
        printf("Error opening file: %d\n", errnum);
        printf("%s\n", strerror(errnum));
    }

    return result;
}

int main(int argc, const char ** argv)
{
    if (argc<2)
    {
        printf("usage: bg2json input.bg2");
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

    // TODO: write json file
    
    bg2io_freeBg2File(fileData);
    free(fileBuffer);
    
    return 0;
}
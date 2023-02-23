
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "bg2-io.h"

void writeObj(Bg2File * fileData, const char * path);

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

    writeObj(fileData, argv[1]);
    
    bg2io_freeBuffer(fileBuffer);
    free(fileBuffer);
    
    bg2io_freeBg2File(fileData);
    free(fileData);
    
    return 0;
}

void writeHeader(FILE * fd, Bg2File * fileData)
{
    fprintf(fd, "# bg2-io library\n");
    fprintf(fd, "# bg2 to obj exporter\n");
    fprintf(fd, "# https://github.com/ferserc1/bg2-io.git\n");
}

int writeFloat3Array(FILE * fd, Bg2ioFloatArray * array, const char * prefix)
{
    if (array->length % 3 != 0) {
        // ERROR: unexpected number of vector components
        return -1;
    }
    for (int i = 0; i < array->length; i += 3)
    {
        float x = array->data[i];
        float y = array->data[i + 1];
        float z = array->data[i + 2];
        fprintf(fd, "%s %f %f %f\n", prefix, x, y, z);
    }

    return 0;
}

int writeFloat2Array(FILE * fd, Bg2ioFloatArray * array, const char * prefix)
{
    if (array->length % 2 != 0) {
        // ERROR: unexpected number of vector components
        return -1;
    }
    for (int i = 0; i < array->length; i += 2)
    {
        float x = array->data[i];
        float y = array->data[i + 1];
        fprintf(fd, "%s %f %f\n", prefix, x, y);
    }

    return 0;
}

int writeFaces(FILE * fd, Bg2ioIntArray * array, int lastIndex)
{
    if (array->length % 3 != 0) {
        // ERROR: unexpected number of face indexes
        return -1;
    }
    for (int i = 0; i < array->length; i += 3)
    {
        int a = array->data[i] + 1 + lastIndex;
        int b = array->data[i + 1] + 1 + lastIndex;
        int c = array->data[i + 2] + 1 + lastIndex;
        fprintf(fd, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", a, a, a, b, b, b, c, c, c);
    }
    return 0;
}

int writePolyLists(FILE * fd, Bg2File * fileData)
{
    int result = 0;
    int lastIndex = 0;
    for (int i = 0; i < fileData->plists->length; ++i)
    {
        Bg2ioPolyList * plist = fileData->plists->data[i];
        fprintf(fd, "o %s\n", plist->name);

        result = writeFloat3Array(fd, &plist->vertex, "v");
        if (result)
        {
            break;
        }
        result = writeFloat3Array(fd, &plist->normal, "vn");
        if (result)
        {
            break;
        }
        result = writeFloat2Array(fd, &plist->texCoord0, "vt");
        if (result)
        {
            break;
        }
        result = writeFaces(fd, &plist->index, lastIndex);
        if (result)
        {
            break;
        }

        lastIndex += (plist->vertex.length / 3);
    }

    return result;
}

void writeObj(Bg2File * fileData, const char * path)
{
    int pathLen = (int) strlen(path);
    char * jsonPath = (char *)malloc(pathLen + 5);  // path length + ".obj\0"
    strcpy(jsonPath, path);
    strcat(jsonPath, ".obj");

    printf("Writting ouput file '%s'\n",jsonPath);

    FILE * fd = fopen(jsonPath, "wt");
    if (fd)
    {
        writeHeader(fd, fileData);
        writePolyLists(fd, fileData);
        fclose(fd);
    }
    else
    {
        printf("Error writting output file at '%s'", jsonPath);
    }
}

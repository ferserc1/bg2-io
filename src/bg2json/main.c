
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "bg2-io.h"

void writeHeader(FILE *fd, Bg2File *fileData);
void writeMaterials(FILE *fd, Bg2File *fileData);
void writeJoints(FILE *fd, Bg2File *fileData);
void writeComponents(FILE *fd, Bg2File *fileData);
void writePolyLists(FILE *fd, Bg2File *fileData);
void writeFloatVector(FILE *fd, Bg2ioFloatArray *array);
void writeIntVector(FILE *fd, Bg2ioIntArray *array);
void writeJson(Bg2File * fileData, const char * path);

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

    writeJson(fileData, argv[1]);
    
    bg2io_freeBuffer(fileBuffer);
    free(fileBuffer);
    
    bg2io_freeBg2File(fileData);
    free(fileBuffer);
    
    return 0;
}

void writeHeader(FILE *fd, Bg2File *fileData)
{
    fprintf(fd, "  \"header\": {\n");
    fprintf(fd, "    \"endianess\": %d,\n", fileData->header.endianess);
    fprintf(fd, "    \"version\": {\n");
    fprintf(fd, "      \"major\": %d,\n", fileData->header.majorVersion);
    fprintf(fd, "      \"minor\": %d,\n", fileData->header.minorVersion);
    fprintf(fd, "      \"revision\": %d\n", fileData->header.revision);
    fprintf(fd, "    },\n");
    fprintf(fd, "    \"numberOfPolyList\": %d\n", fileData->header.numberOfPolyList);
    fprintf(fd, "  },\n");
}

void writeMaterials(FILE *fd, Bg2File *fileData)
{
    fprintf(fd, "  \"materialData\": %s,\n", fileData->materialData);
}

void writeJoints(FILE *fd, Bg2File *fileData)
{
    if (fileData->jointData != NULL)
    {
        fprintf(fd, "  \"joints\": %s,\n", fileData->jointData);
    }
}

void writeComponents(FILE *fd, Bg2File *fileData)
{
    if (fileData->componentData != NULL)
    {
        fprintf(fd, "  \"componentData\": %s,\n", fileData->componentData);
    }
}

void writePolyLists(FILE *fd, Bg2File *fileData)
{
    fprintf(fd, "  \"polyLists\": [\n");
    for (int i = 0; i < fileData->plists->length; ++i)
    {
        Bg2ioPolyList * plist = fileData->plists->data[i];
        fprintf(fd, "    {\n");
        fprintf(fd, "      \"name\": \"%s\",\n", plist->name);
        fprintf(fd, "      \"matName\": \"%s\"", plist->matName);
        if (plist->vertex.data != NULL)
        {
            fprintf(fd, ",\n");
            fprintf(fd, "      \"vertex\": "); writeFloatVector(fd, &plist->vertex);
        }

        if (plist->normal.data != NULL)
        {
            fprintf(fd, ",\n");
            fprintf(fd, "      \"normal\": "); writeFloatVector(fd, &plist->normal);
        }

        if (plist->texCoord0.data != NULL)
        {
            fprintf(fd, ",\n");
            fprintf(fd, "      \"texCoord0\": "); writeFloatVector(fd, &plist->texCoord0);
        }

        if (plist->texCoord1.data != NULL)
        {
            fprintf(fd, ",\n");
            fprintf(fd, "      \"texCoord1\": "); writeFloatVector(fd, &plist->texCoord1);
        }

        if (plist->texCoord2.data != NULL)
        {
            fprintf(fd, ",\n");
            fprintf(fd, "      \"texCoord2\": "); writeFloatVector(fd, &plist->texCoord2);
        }

        if (plist->index.data != NULL)
        {
            fprintf(fd, ",\n");
            fprintf(fd, "      \"index\": "); writeIntVector(fd, &plist->index);
        }

        fprintf(fd, "\n    }");

        if (i < fileData->plists->length - 1)
        {
            fprintf(fd, ",\n");
        }
        else
        {
            fprintf(fd, "\n");
        }
    }
    fprintf(fd, "  ]\n");
}

void writeFloatVector(FILE *fd, Bg2ioFloatArray *array)
{
    fprintf(fd, "[");
    for (int i = 0; i < array->length; ++i)
    {
        fprintf(fd, "%f", array->data[i]);

        if (i < array->length - 1)
        {
            fprintf(fd, ",");
        }
    }
    fprintf(fd, "]");
}

void writeIntVector(FILE *fd, Bg2ioIntArray *array)
{
    fprintf(fd, "[");
    for (int i = 0; i < array->length; ++i)
    {
        fprintf(fd, "%d", array->data[i]);

        if (i < array->length - 1)
        {
            fprintf(fd, ",");
        }
    }
    fprintf(fd, "]");
}

void writeJson(Bg2File * fileData, const char * path)
{
    int pathLen = strlen(path);
    char * jsonPath = (char *)malloc(pathLen + 6);  // path length + ".json\0"
    strcpy(jsonPath, path);
    strcat(jsonPath, ".json");

    printf("Writting ouput file '%s'\n",jsonPath);

    FILE * fd = fopen(jsonPath, "wt");
    if (fd)
    {
        fprintf(fd, "{\n");
        writeHeader(fd, fileData);
        writeMaterials(fd, fileData);
        writeJoints(fd, fileData);
        writeComponents(fd, fileData);
        writePolyLists(fd, fileData);
        fprintf(fd, "}");

        fclose(fd);
    }
    else
    {
        printf("Error writting output file at '%s'", jsonPath);
    }
}
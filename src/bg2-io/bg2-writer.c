
#include "bg2-writer.h"
#include "errors.h"
#include <string.h>

// String size in buffer: 4 bytes for the string size and 1 byte for each character
#define STR_SIZE(s)  sizeof(char) * strlen(s) + sizeof(int)
// Array size in buffer: 4 bytes for the buffer size and 4 bytes for each element
#define ARR_SIZE(v,T)  v.length * sizeof(T) + sizeof(int)

Bg2ioSize calculatePolyListSize(Bg2File * file)
{
    if (file->plists == NULL || file->plists->length == 0)
    {
        return BG2IO_ERR_INVALID_PLIST_DATA;
    }

    Bg2ioSize total = 0;

    for (int i = 0; i < file->plists->length; ++i)
    {
        Bg2ioPolyList * plist = file->plists->data[i];
        if (plist->vertex.length == 0)
        {
            return BG2IO_ERR_INVALID_VERTEX_DATA;
        }
        if (plist->index.length == 0)
        {
            return BG2IO_ERR_INVALID_INDEX_DATA;
        }

        total += 4; // plst block

        if (plist->name != NULL)
        {
            // pnam block (4 bytes) + name
            total += 4 + STR_SIZE(plist->name);
        }
        if (plist->matName != NULL)
        {
            // mnam block (4 bytes) + matName
            total += 4 + STR_SIZE(plist->matName);
        }
        // varr block (4 bytes) + vertex array length
        total += 4 + ARR_SIZE(plist->vertex, float); 
        if (plist->normal.length > 0)
        {
            // narr block (4 bytes) + normal array length
            total += 4 + ARR_SIZE(plist->normal, float);
        }
        if (plist->texCoord0.length > 0)
        {
            // t0ar block (4 bytes) + array length
            total += 4 + ARR_SIZE(plist->texCoord0, float);
        }
        if (plist->texCoord1.length > 0)
        {
            // t1ar block (4 bytes) + array length
            total += 4 + ARR_SIZE(plist->texCoord1, float);
        }
        if (plist->texCoord2.length > 0)
        {
            // t0ar block (4 bytes) + array length
            total += 4 + ARR_SIZE(plist->texCoord2, float);
        }
        //  indx block (4 bytes) +  array length
        total += 4 + ARR_SIZE(plist->index, int);
    }

    // end4 block: 4 bytes
    total += 4;

    return total;
}

Bg2ioSize bg2io_calculateBufferSize(Bg2File * file)
{
    if (file == NULL)
    {
        return BG2IO_ERR_INVALID_FILE_PTR;
    }
    else if (file->materialData == NULL)
    {
        return BG2IO_ERR_INVALID_MATERIAL_DATA;
    }

    // Header: endianess, major version, minor version, revision: 4 bytes
    // hedr block: 4 bytes
    // Number of poly lists: 4 bytes
    // mtrl block: 4 bytes
    Bg2ioSize total = 16;

    // Material string
    total += STR_SIZE(file->materialData);

    // Joint block: 4 bytes
    // Joint string
    if (file->jointData != NULL)
    {
        total += 4 + STR_SIZE(file->jointData);
    }
    else
    {
        // Joint data is optional in Bg2File struct, but mandatory on file buffer
        total += 4 + STR_SIZE("{}");
    }

    Bg2ioSize plistSize = calculatePolyListSize(file);
    if (plistSize < 0)
    {
        return plistSize;   // Error
    }
    total += plistSize;

    // components (if present)
    if (file->componentData != NULL)
    {
        // cmps block (4 bytes) + block string
        total += 4 + STR_SIZE(file->componentData);
    }

    return total;
}

int bg2io_writeFileToBuffer(Bg2File * file, Bg2ioBuffer *dest)
{
    if (dest == NULL)
    {
        return BG2IO_ERR_INVALID_BUFFER_PTR;
    }

    Bg2ioSize requiredSize = bg2io_calculateBufferSize(file);
    if (requiredSize<0)
    {
        return requiredSize;
    }

    return BG2IO_NO_ERROR;
}

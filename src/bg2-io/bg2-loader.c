
#include "bg2-loader.h"
#include "errors.h"
#include "buffer-io.h"

static int g_parseError;

int parseHeader(Bg2ioBufferIterator *it, Bg2File *file);

int bg2io_getParserError()
{
    return g_parseError;
}

Bg2File * bg2io_parseFileBuffer(Bg2ioBuffer * buffer)
{
    g_parseError = BG2IO_NO_ERROR;

    if (buffer == NULL)
    {
        g_parseError = BG2IO_ERR_INVALID_PTR;
        return NULL;
    }

    Bg2File * file = bg2io_createBg2File();
    Bg2ioBufferIterator it = BG2IO_ITERATOR(buffer);

    if (parseHeader(&it, file) != BG2IO_NO_ERROR)
    {
        bg2io_freeBg2File(file);
        return NULL;
    }

    return file;
}

int parseHeader(Bg2ioBufferIterator *it, Bg2File *file)
{
    // Read endianess and version
    if (bg2io_readByte(it, &file->header.endianess) < 0 ||
        bg2io_readByte(it, &file->header.majorVersion) < 0 ||
        bg2io_readByte(it, &file->header.minorVersion) < 0 ||
        bg2io_readByte(it, &file->header.revision) < 0)
    {
        g_parseError = BG2IO_ERR_INVALID_FILE_HEADER;
        return g_parseError;
    }

    // Read header block tag
    int block = bg2io_readBlock(it);
    if (block == BG2IO_ERR_INVALID_BLOCK)
    {
        g_parseError = block;
        return g_parseError;
    }
    else if (block != bg2io_Header)
    {
        g_parseError = BG2IO_ERR_UNEXPECTED_BLOCK;
        return g_parseError;
    }

    // Read number of poly list elements
    if (bg2io_readInteger(it, &file->header.numberOfPolyList) < 0)
    {
        g_parseError = BG2IO_ERR_NUMBER_OF_PLIST_EXPECTED;
        return g_parseError;
    }

    // Read materials
    block = bg2io_readBlock(it);
    if (block == BG2IO_ERR_INVALID_BLOCK)
    {
        g_parseError = BG2IO_ERR_MATERIAL_DATA_EXPECTED;
        return g_parseError;
    }

    if (bg2io_readString(it, &file->materialData) < 0)
    {
        g_parseError = BG2IO_ERR_CORRUPTED_MATERIAL_DATA;
        return g_parseError;
    }
    
    block = bg2io_readBlock(it);
    if (block == BG2IO_ERR_INVALID_BLOCK)
    {
        g_parseError = BG2IO_ERR_JOINT_BLOCK_EXPECTED;
        return g_parseError;
    }

    // Read projectors: deprecated, this section only skips the section
    if (block == bg2io_ShadowProjector)
    {
        // Shadow text file: read string to skip the string
        int stringSize = 0;
        bg2io_readInteger(it, &stringSize);
        it->current += stringSize;

        // Attenuation
        it->current += sizeof(float);

        // Projection matrix
        it->current += sizeof(float) * 16;

        // View matrix
        it->current += sizeof(float) * 16;

        // Read next block
        block = bg2io_readBlock(it);
    }
    
    // Read joints
    if (block == bg2io_Joint)
    {
        int err = bg2io_readString(it, &file->jointData);
        if (err < 0)
        {
            g_parseError = BG2IO_ERR_CORRUPTED_JOINT_DATA;
            return g_parseError;
        }
    }

    return BG2IO_NO_ERROR;
}

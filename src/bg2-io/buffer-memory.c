
#include "buffer-memory.h"

#define BUFFER_BLOCK_SIZE 4096

Bg2ioSize bg2io_getActualBufferSize(Bg2ioSize requiredSize)
{
    Bg2ioSize remainder = requiredSize % BUFFER_BLOCK_SIZE;
    if (remainder == 0)
    {
        return requiredSize;
    }

    return requiredSize + BUFFER_BLOCK_SIZE - remainder;
}

Bg2ioSize bg2io_createBuffer(Bg2ioBuffer *in, Bg2ioSize requiredSize)
{
    if (in == NULL)
    {
        return -1;
    }
    else if (in->buffer != NULL || in->actualLength > 0 || in->length > in->actualLength) {
        return -2;
    }

    in->actualLength = bg2io_getActualBufferSize(requiredSize);
    in->length = requiredSize;
    in->buffer = malloc(sizeof(Bg2ioByte) * in->actualLength);
    return in->actualLength;
}

Bg2ioSize bg2io_reserveBuffer(Bg2ioBuffer *buffer, Bg2ioSize requiredSize)
{
    if (buffer == NULL)
    {
        return -1;
    }
    else if (requiredSize < buffer->length)
    {
        return -2;
    }
    else if (requiredSize < buffer->actualLength)
    {
        buffer->length = requiredSize;
        return buffer->actualLength;
    }
    else
    {
        // Store the previous buffer pointer and size
        Bg2ioBytePtr oldBuffer = buffer->buffer;
        Bg2ioSize oldLength = buffer->length;

        // Allocate the new buffer
        buffer->actualLength = bg2io_getActualBufferSize(requiredSize);
        buffer->length = requiredSize;
        buffer->buffer = malloc(sizeof(Bg2ioBuffer) * buffer->actualLength);
        
        // Copy the old buffer to the new one
        for (Bg2ioSize i = 0; i < oldLength; ++i)
        {
            buffer->buffer[i] = oldBuffer[i];
        }

        // Release the old buffer memory
        free(oldBuffer);
        return buffer->actualLength;
    }
}

void bg2io_freeBuffer(Bg2ioBuffer *buffer)
{
    if (buffer != NULL && buffer->buffer != NULL)
    {
        free(buffer->buffer);
        buffer->buffer = NULL;
        buffer->length = 0;
        buffer->actualLength = 0;
    }
}

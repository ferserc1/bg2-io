
#include "buffer-io.h"
#include "errors.h"
#include "bg2-io.h"

#include <string.h>

Bg2ioSize checkIterator(Bg2ioBufferIterator *it, Bg2ioSize requiredBytes)
{
    if (it == NULL || it->buffer == NULL || it->buffer->mem == NULL)
	{
		return BG2IO_ERR_INVALID_PTR;
	}
	else if (it->current >= it->buffer->length)
	{
		return BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS;
	}
	else if ((it->current + requiredBytes) > it->buffer->length)
	{
		return BG2IO_ERR_INSUFFICIENT_LENGTH;
	}
    else
    {
        return BG2IO_NO_ERROR;
    }
}

void checkAppendDataLength(Bg2ioBufferIterator *it, Bg2ioSize requiredSize)
{
    Bg2ioSize requiredTotalLength = it->current + requiredSize;
    if (requiredTotalLength > it->buffer->length)
    {
        bg2io_reserveBuffer(it->buffer, requiredTotalLength);
    }
}

Bg2ioSize bg2io_readByte(Bg2ioBufferIterator *it, unsigned char *out)
{
    Bg2ioSize error = checkIterator(it, sizeof(Bg2ioByte));
    if (error != BG2IO_NO_ERROR)
    {
        return error;
    }
    else if (out == NULL)
    {
        return BG2IO_ERR_INVALID_OUT_PARAM_PTR;
    }

	long long cur = it->current;
	*out = it->buffer->mem[cur];
	it->current += sizeof(Bg2ioByte);
    return it->buffer->length - it->current; 
}

Bg2ioSize bg2io_readInteger(Bg2ioBufferIterator *it, int *out)
{
    Bg2ioSize readed = checkIterator(it, sizeof(int));
    if (readed != BG2IO_NO_ERROR)
    {
        return readed;
    }
    else if (out == NULL)
    {
        return BG2IO_ERR_INVALID_OUT_PARAM_PTR;
    }

    if (bg2io_isBigEndian())
    {
        memcpy((void *)out, it->buffer->mem, sizeof(int));
    }
    else
    {
        union {
            int integer;
            char byte[4];
        } r_value;
        long long cur = it->current;
        r_value.byte[3] = it->buffer->mem[cur];
        r_value.byte[2] = it->buffer->mem[cur + 1];
        r_value.byte[1] = it->buffer->mem[cur + 2];
        r_value.byte[0] = it->buffer->mem[cur + 3];
        *out = r_value.integer;
    }
    it->current += sizeof(int);
    readed = it->buffer->length - it->current;
    return readed;
}

Bg2ioSize bg2io_writeByte(Bg2ioBufferIterator *it, const unsigned char in)
{
    Bg2ioSize error = checkIterator(it, sizeof(int));
    // Ignore insufficient length error, because if it is the case, the
    // checkAppendDataLength() function will increase the buffer size.
    if (error != BG2IO_NO_ERROR && error != BG2IO_ERR_INSUFFICIENT_LENGTH)
    {
        return error;
    }

    Bg2ioSize increment = sizeof(unsigned char);
    checkAppendDataLength(it, increment);
    it->buffer->mem[it->current] = in;
    it->current += increment;
    return increment;
}

Bg2ioSize bg2io_writeInteger(Bg2ioBufferIterator *it, int in)
{
    Bg2ioSize error = checkIterator(it, sizeof(int));
    // Ignore insufficient length error, because if it is the case, the
    // checkAppendDataLength() function will increase the buffer size.
    if (error != BG2IO_NO_ERROR && error != BG2IO_ERR_INSUFFICIENT_LENGTH)
    {
        return error;
    }

    Bg2ioSize increment = sizeof(int);
    checkAppendDataLength(it, increment);
    
    if (bg2io_isBigEndian())
    {
        memcpy((void*)&it->buffer[it->current], (void*) &in, increment);
    }
    else
    {
        union {
            int integer;
            unsigned char bytes[4];
        } r_value;
        r_value.integer = in;
        it->buffer->mem[it->current] = r_value.bytes[3];
        it->buffer->mem[it->current + 1] = r_value.bytes[2];
        it->buffer->mem[it->current + 2] = r_value.bytes[1];
        it->buffer->mem[it->current + 3] = r_value.bytes[0];
    }

    it->current += increment;
    return increment;
}


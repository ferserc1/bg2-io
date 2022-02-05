
#include "buffer-io.h"
#include "errors.h"

Bg2ioSize bg2io_readByte(Bg2ioBufferIterator *it, unsigned char *out)
{
	if (it == NULL || it->buffer == NULL || it->buffer->mem == NULL)
	{
		return BG2_IO_ERR_INVALID_PTR;
	}
	else if (it->current >= it->buffer->length)
	{
		return BG2_IO_ERR_ITERATOR_OUT_OF_BOUNDS;
	}
	else if ((it->current + sizeof(Bg2ioByte)) == it->buffer->length)
	{
		return BG2_IO_ERR_INSUFFICIENT_LENGTH;
	}

	long long cur = it->current;
	*out = it->buffer->mem[cur];
	it->current += sizeof(Bg2ioByte);
    return it->buffer->length - it->current; 
}

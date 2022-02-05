
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bg2-io.h"

void dumpBuffer(Bg2ioBuffer *buffer)
{
	Bg2ioBufferIterator it = BG2IO_BUFFER_ITERATOR_INIT;
	it.buffer = buffer;
	unsigned char byte = '\0';
	while (bg2io_readByte(&it, &byte) > 0)
	{
		printf("%c", byte);
	}
	printf("\n");
}

int readBuffer(const char * path, Bg2ioBuffer *buffer)
{
	FILE * file;
	int errnum;

	file = fopen(path, "rb");
	if (file != NULL)
	{
		fseek(file, 0, SEEK_END);
		Bg2ioSize fileLength = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (bg2io_createBuffer(buffer, fileLength) < 0)
		{
			printf("Error: could not allocate memory");
			return -2;
		}
		else
		{
			fread(buffer->mem, buffer->length, 1, file);
		}

		fclose(file);

		return 0;
	}
	else
	{
		errnum = errno;
		printf("Value of errno: %d\n", errno);
		printf("Error: %s\n", strerror(errno));
		return -1;
	}
}

int main(int argc, const char ** argv)
{
	if (argc<2) {
		printf("usage: sample file_path.bg2\n");
		exit(1);
	}

	Bg2ioBuffer buffer = BG2IO_BUFFER_INIT;
	if (readBuffer(argv[1], &buffer) != 0)
	{
		printf("Error: could not read file at path \"%s\"\n", argv[1]);
		return -1;
	}
	else
	{
		dumpBuffer(&buffer);

		Bg2ioBufferIterator it = BG2IO_ITERATOR(&buffer);
		unsigned char endian = 0;
		unsigned char major = 0;
		unsigned char minor = 0;
		unsigned char rev = 0;
		bg2io_readByte(&it, &endian);
		bg2io_readByte(&it, &major);
		bg2io_readByte(&it, &minor);
		bg2io_readByte(&it, &rev);

		unsigned int header = 0;
		bg2io_readInteger(&it, &header);

		printf("Version: %d.%d.%d\n", major, minor, rev);

		if (header == bg2io_Header)
		{
			printf("Header\n");
		}

		bg2io_freeBuffer(&buffer);
	}

	if (bg2io_isBigEndian())
	{
		printf("Big endian platform\n");
	}
	else
	{
		printf("Little endian platform\n");
	}

	if (bg2io_isLittleEndian())
	{
		printf("Little endian platform\n");
	}
	else
	{
		printf("Big endian platform\n");
	}

	return 0;
}

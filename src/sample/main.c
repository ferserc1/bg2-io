
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
	while (bg2io_readByte(&it, &byte) >= 0)
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

void testWrite()
{
	Bg2ioBuffer buffer = BG2IO_BUFFER_INIT;
	Bg2ioSize size = sizeof(unsigned char) * 3 + sizeof(int) * 4;
	bg2io_createBuffer(&buffer, size);


	Bg2ioBufferIterator it = BG2IO_ITERATOR(&buffer);

	union {
		int integer;
		char bytes[4];
	} cosa;
	cosa.integer = 'cosa';
	float v1= -3.141592f;
	float v2 = 55.0f;
	float v3 = 2e6f;
	bg2io_writeByte(&it, 'a');
	bg2io_writeByte(&it, 'b');
	bg2io_writeByte(&it, 'c');
	bg2io_writeInteger(&it, 0xFF);
	bg2io_writeInteger(&it, 0xFF00);
	bg2io_writeInteger(&it, 0xFF0000);
	bg2io_writeInteger(&it, 0x7F000000);
	bg2io_writeInteger(&it, cosa.integer);
	bg2io_writeFloat(&it, v1);
	bg2io_writeFloat(&it, v2);
	bg2io_writeFloat(&it, v3);

	bg2io_writeString(&it, "bg2 engine input/output tools.");

	it.current = 0;
	char bytes[3];
	int integers[4];
	float floats[3];
	char * str;
	
	bg2io_readByte(&it, &bytes[0]);
	bg2io_readByte(&it, &bytes[1]);
	bg2io_readByte(&it, &bytes[2]);
	bg2io_readInteger(&it, &integers[0]);
	bg2io_readInteger(&it, &integers[1]);
	bg2io_readInteger(&it, &integers[2]);
	bg2io_readInteger(&it, &integers[3]);
	bg2io_readInteger(&it, &cosa.integer);
	bg2io_readFloat(&it, &floats[0]);
	bg2io_readFloat(&it, &floats[1]);
	bg2io_readFloat(&it, &floats[2]);
	bg2io_readString(&it, &str);

	printf("%c, %c, %c, %#010x, %#010x, %#010x, %#010x\n", bytes[0], bytes[1], bytes[2], integers[0], integers[1], integers[2], integers[3]);
	printf("%c%c%c%c\n", cosa.bytes[0], cosa.bytes[1], cosa.bytes[2], cosa.bytes[3]);
	printf("%f\n%f\n%f\n", v1, v2, v3);
	printf("%s\n", str);

	dumpBuffer(it.buffer);
}

int testFiles(const char * path)
{
	Bg2ioBuffer buffer = BG2IO_BUFFER_INIT;
	if (readBuffer(path, &buffer) != 0)
	{
		printf("Error: could not read file at path \"%s\"\n", path);
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
		return 0;
	}
}

void testEndianness()
{
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
}

int main(int argc, const char ** argv)
{
	if (argc<2) {
		printf("usage: sample file_path.bg2\n");
		exit(1);
	}

	//testFiles(argv[1]);
	testEndianness();
	testWrite();

	return 0;
}

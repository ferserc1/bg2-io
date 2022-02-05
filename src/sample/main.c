
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bg2-io.h"

void dumpBuffer(Bg2ioBuffer *buffer)
{
	Bg2ioSize i;
	for (i = 0; i < buffer->length; ++i)
	{
		printf("%c", ((char *)buffer->buffer)[i]);
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
			fread(buffer->buffer, buffer->length, 1, file);
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
		bg2io_freeBuffer(&buffer);
	}
	return 0;
}

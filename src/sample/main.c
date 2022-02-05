
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bg2-io.h"

typedef struct Buffer {
	char * buffer;
	unsigned long long length;
} Buffer;

void dumpBuffer(void *buffer, int bufferSize)
{
	int i;
	for (i = 0; i < bufferSize; ++i)
	{
		printf("%c", ((char *)buffer)[i]);
	}
	printf("\n");
}

Buffer * readBuffer(const char * path)
{
	FILE * file;
	int errnum;
	Buffer * result = NULL;

	file = fopen(path, "rb");
	if (file != NULL)
	{
		result = malloc(sizeof(Buffer));
		fseek(file, 0, SEEK_END);
		result->length = ftell(file);
		fseek(file, 0, SEEK_SET);

		result->buffer = (char *) malloc(result->length);

		if (!result->buffer)
		{
			printf("Error: could not allocate memory");
			free(result);
			result = NULL;
		}
		else
		{
			fread(result->buffer, result->length, 1, file);
		}

		fclose(file);
	}
	else
	{
		errnum = errno;
		printf("Value of errno: %d\n", errno);
		printf("Error: %s\n", strerror(errno));
	}
	
	return result;
}

void freeBuffer(Buffer *b) {
	if (b != NULL)
	{
		if (b->buffer != NULL)
		{
			free(b->buffer);
		}
		free(b);
	}
}

int main(int argc, const char ** argv)
{
	if (argc<2) {
		printf("usage: sample file_path.bg2\n");
		exit(1);
	}

	Buffer * data = readBuffer(argv[1]);
	if (data == NULL)
	{
		printf("Error: could not read file at path \"%s\"\n", argv[1]);
		return -1;
	}
	else
	{
		dumpBuffer(data->buffer, data->length);
		freeBuffer(data);
	}
	return 0;
}

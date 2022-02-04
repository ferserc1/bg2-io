
#include <stdio.h>
#include <stdlib.h>
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

int main()
{
	const char * path = "/home/fernando/desarrollo/bg2-io/resources/test.txt";
	const char * path2 = "c:\\Users\\ferse\\Desktop\\resources\\test.txt";
	Buffer * data = readBuffer(path2);

	if (data == NULL)
	{
		printf("Error: could not read file at path \"%s\"\n", path);
		return -1;
	}
	else
	{
		dumpBuffer(data->buffer, data->length);
		free(data);
	}
	return 0;
}


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

	free(str);

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

		int numberOfPlist = 0;
		bg2io_readInteger(&it, &numberOfPlist);
		printf("Number of polyList: %d\n", numberOfPlist);

		unsigned int token = 0;
		bg2io_readInteger(&it, &token);

		if (token == bg2io_Materials)
		{
			printf("Materials block\n");
		}

		char * headerString;
		bg2io_readString(&it, &headerString);
		printf("%s\n", headerString);

		int block = 0;
		bg2io_readInteger(&it, &block);
		if (block == bg2io_ShadowProjector)
		{
			// Deprecated: this block skips the projector section
			char * fileName;
			float proj[16];
			float trans[16];
			float attenuation;
			bg2io_readString(&it, &fileName);
			bg2io_readFloat(&it, &attenuation);
			for (int i = 0; i<16; ++i)
			{
				bg2io_readFloat(&it, &proj[i]);
			}
			for (int i = 0; i<16; ++i)
			{
				bg2io_readFloat(&it, &trans[i]);
			}
			free(fileName);
		}
		else
		{
			it.current -= 4;
		}

		// Joint list
		bg2io_readInteger(&it, &block);
		if (block == bg2io_Joint)
		{
			char * jointList;
			bg2io_readString(&it, &jointList);
			free(jointList);
		}
		else
		{
			it.current -= 4;
		}

		// Read poly lists
		bg2io_readInteger(&it, &block);
		if (block != bg2io_PolyList)
		{
			// Format error
		}
		else
		{
			for (int i = 0; i<numberOfPlist; ++i)
			{
				char * plistName = NULL;
				char * matName = NULL;
				float * vertex = NULL;
				int numVertex = 0;
				float * normal = NULL;
				int numNormals = 0;
				float * t0 = NULL;
				int numT0 = 0;				
				float * t1 = NULL;
				int numT1 = 0;
				float * t2 = NULL;
				int numT2 = 0;
				int * index = NULL;
				int numIndex = 0;
				int done = 0;
				while (done == 0)
				{
					// Read single poly list
					bg2io_readInteger(&it, &block);
					switch (block) {
					case bg2io_PlistName:
						bg2io_readString(&it, &plistName);
						break;
					case bg2io_MatName:
						bg2io_readString(&it, &matName);
						break;
					case bg2io_VertexArray:
						numVertex = bg2io_readFloatArray(&it, &vertex);
						break;
					case bg2io_NormalArray:
						numNormals = bg2io_readFloatArray(&it, &normal);
						break;
					case bg2io_TexCoord0Array:
						numT0 = bg2io_readFloatArray(&it, &t0);
						break;
					case bg2io_TexCoord1Array:
						numT1 = bg2io_readFloatArray(&it, &t1);
						break;
					case bg2io_TexCoord2Array:
						numT2 = bg2io_readFloatArray(&it, &t2);
						break;
					case bg2io_IndexArray:
						numIndex = bg2io_readIntArray(&it, &index);
						break;
					case bg2io_PolyList:
					case bg2io_End:
						// Done: add a poly list
						if (vertex != NULL)
						{
							free(vertex);
							vertex = NULL;
							numVertex = 0;
						}
						if (normal != NULL)
						{
							free(normal);
							normal = NULL;
							numNormals = 0;
						}
						if (t0 != NULL)
						{
							free(t0);
							t0 = NULL;
							numT0 = 0;
						}
						if (t1 != NULL)
						{
							free(t1);
							t1 = NULL;
							numT1 = 0;
						}
						if (t2 != NULL)
						{
							free(t2);
							t2 = NULL;
							numT2 = 0;
						}
						if (index != NULL)
						{
							free(index);
							index = NULL;
							numIndex = 0;
						}
						if (plistName != NULL)
						{
							free(plistName);
							plistName = NULL;
						}
						if (matName != NULL)
						{
							free(matName);
							matName = NULL;
						}

						if (block == bg2io_End)
						{
							done = 1;
						}
						break;
					}
				}

			}
		}

		free(headerString);
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

	testFiles(argv[1]);
	testEndianness();
	testWrite();

	return 0;
}

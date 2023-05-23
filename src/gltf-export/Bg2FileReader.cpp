
#include <Bg2FileReader.hpp>

#include <fstream>
#include <stdexcept>

#include <stdlib.h>
#include <stdio.h>

Bg2FileReader::Bg2FileReader()
{

}

Bg2FileReader::~Bg2FileReader()
{
	close();
}

void Bg2FileReader::open(const std::string& path)
{
	if (_bg2File != nullptr)
	{
		close();
	}

	FILE* fd;
	fd = fopen(path.c_str(), "rb");
	if (!fd)
	{
		throw std::runtime_error("Could not open file " + path);
	}

	fseek(fd, 0, SEEK_END);
	Bg2ioSize size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	Bg2ioBuffer* buffer = (Bg2ioBuffer*) malloc(sizeof(Bg2ioBuffer));
	BG2IO_BUFFER_PTR_INIT(buffer);

	if (bg2io_createBuffer(buffer, size) < 0)
	{
		throw std::runtime_error("Could not allocate memory to store input file buffer");
	}
	else
	{
		fread(buffer->mem, buffer->length, 1, fd);
		fclose(fd);

		_bg2File = bg2io_parseFileBuffer(buffer);

		if (!_bg2File)
		{
			auto errorNo = bg2io_getParserError();
			throw std::runtime_error("Error parsing bg2 file: " + std::to_string(errorNo));
		}

		bg2io_freeBuffer(buffer);
		free(buffer);
	}
}

void Bg2FileReader::close()
{
	if (_bg2File != nullptr)
	{
		bg2io_freeBg2File(_bg2File);
		free(_bg2File);
		_bg2File = nullptr;
	}
}
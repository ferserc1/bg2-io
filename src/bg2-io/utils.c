
#include "utils.h"

int bg2io_isBigEndian()
{
	union {
		unsigned char bytes[4];
		unsigned int word;
	} EndianCheck;
	EndianCheck.word = 0x01234567;

	if (EndianCheck.bytes[0] == 0x67)
	{
		return 0;	
	}
	else
	{
		return 1;
	}
}

int bg2io_isLittleEndian()
{
	union {
		unsigned char bytes[4];
		unsigned int word;
	} EndianCheck;
	EndianCheck.word = 0x01234567;
	
	if (EndianCheck.bytes[0] == 0x67)
	{
		return 1;	
	}
	else
	{
		return 0;
	}
}

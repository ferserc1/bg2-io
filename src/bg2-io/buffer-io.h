#ifndef BG2IO_BUFFER_IO_H
#define BG2IO_BUFFER_IO_H

#include "types.h"

enum bg2ioBufferType {
    bg2io_Header = 'hedr',
    bg2io_PolyList = 'plst',
    bg2io_VertexArray = 'varr',
    bg2io_NormalArray = 'narr',
    bg2io_TexCoord0Array = 't0ar',
    bg2io_TexCoord1Array = 't1ar',
    bg2io_TexCoord2Array = 't2ar',
    bg2io_TexCoord3Array = 't3ar',
    bg2io_TexCoord4Array = 't4ar',
    bg2io_IndexArray = 'indx',
    bg2io_Materials = 'mtrl',
    bg2io_PlistName = 'pnam',
    bg2io_MatName = 'mnam',
    bg2io_ShadowProjector = 'proj',
    bg2io_Joint = 'join',
    bg2io_End = 'endf'
};

// long long readByte(BufferIterator *buffer, unsigned char *out);


#endif
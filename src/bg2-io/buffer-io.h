#ifndef BG2IO_BUFFER_IO_H
#define BG2IO_BUFFER_IO_H

#include "types.h"

enum Bg2ioBufferType {
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

/**
 * @brief Reads a byte from the buffer iterator and increments it one byte
 * 
 * @param buffer input buffer iterator
 * @param out output byte
 * @return int Returns the remaining buffer size or:
 *  - BG2_IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2_IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position 
 *    is greater or equal than the buffer length
 *  - BG2_IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is null
 *  - BG2_IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 */
Bg2ioSize bg2io_readByte(Bg2ioBufferIterator *it, unsigned char *out);

#endif
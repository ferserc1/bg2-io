#ifndef BG2IO_BUFFER_IO_H
#define BG2IO_BUFFER_IO_H

#include "types.h"

enum Bg2ioBlockType {
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
    bg2io_End = 'endf',
    bg2io_Components = 'cmps'
};

/**
 * @brief Reads a byte from the buffer iterator and increments it one byte
 * 
 * @param it input buffer iterator
 * @param out output byte
 * @return Bg2ioSize Returns the remaining buffer size or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position 
 *    is greater or equal than the buffer length
 *  - BG2IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is NULL
 *  - BG2IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 */
Bg2ioSize bg2io_readByte(Bg2ioBufferIterator *it, unsigned char *out);

/**
 * @brief Read a block from the buffer iterator
 * 
 * @param it 
 * @return unsigned int The reade block, thats it, one of the Bg2ioBlockType enum, or:
 * - BG2IO_ERR_INVALID_BLOCK if the buffer does not contain a valid block at the iterator
 *   position. In this case, the position of the iterator will be reset to the same position 
 *   it was when this function was called.
 * - Any other error code returned by the function bg2io_readInteger()
 * 
 */
unsigned int bg2io_readBlock(Bg2ioBufferIterator *it);

/**
 * @brief Reads an integer value from the buffer iterator, and increments it four bytes
 * 
 * @param it input buffer iterator
 * @param out output byte
 * @param swapBytes
 * @return Bg2ioSize Returns the remaining buffer size of:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater or equal than the buffer length
 *  - BG2IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is NULL
 *  - BG2IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 */
Bg2ioSize bg2io_readInteger(Bg2ioBufferIterator *it, int *out);


/**
 * @brief Reads floating point 32 bit value from the buffer iterator, and increments it four bytes
 * 
 * @param it input buffer iterator
 * @param out output byte
 * @param swapBytes
 * @return Bg2ioSize Returns the remaining buffer size or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater or equal than the buffer length
 *  - BG2IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is NULL
 *  - BG2IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 */
Bg2ioSize bg2io_readFloat(Bg2ioBufferIterator *it, float *out);

/**
 * @brief Reads a string value from buffer iterator. The fromat of the string is a text string
 * Pascal: i.e., the length of the string as a 32-bit integer, followed by the text string
 * 
 * @param it input buffer iterator 
 * @param out output string. This string must be freed with free() when it is no longer used.
 * @return Bg2ioSize Returns the string size or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater or equal than the buffer length
 *  - BG2IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is NULL
 *  - BG2IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 * 
 */
Bg2ioSize bg2io_readString(Bg2ioBufferIterator *it, char **out);

/**
 * @brief Reads a floating point 32 bit array from buffer iterator. The array format consists of
 * a 32-bit integer value indicating the number of elements in the array, followed by as many 
 * 32-bit floating point values as that number indicates.
 * 
 * @param it input buffer iterator 
 * @param out output array. This array must be freed with free() when it is no longer used.
 * @return Bg2ioSize Returns the array size or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater or equal than the buffer length
 *  - BG2IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is NULL
 *  - BG2IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 * 
 */
Bg2ioSize bg2io_readFloatArray(Bg2ioBufferIterator *it, float **out);

/**
 * @brief Reads an integer 32 bit array from buffer iterator. The array format consists of
 * a 32-bit integer value indicating the number of elements in the array, followed by as many 
 * 32-bit integer values as that number indicates.
 * 
 * @param it input buffer iterator 
 * @param out output array. This array must be freed with free() when it is no longer used.
 * @return Bg2ioSize Returns the array size or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater or equal than the buffer length
 *  - BG2IO_ERR_INVALID_OUT_PARAM_PTR if the output parameter is NULL
 *  - BG2IO_ERR_INSUFFICIENT_LENGTH if there are not enough bytes to read from the iterator
 * 
 */
Bg2ioSize bg2io_readIntArray(Bg2ioBufferIterator *it, int **out);

/**
 * @brief Write a byte to a buffer pointed by an iterator, and increment it one byte. If necessary, 
 * increase the buffer size to fit the added data. In this case, the length and actualLength 
 * attributes of the buffer will be increased.
 * 
 * @param it input buffer iterator
 * @param in input value
 * @return Bg2ioSize Returns the written bytes or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater than the buffer length
 * 
 */
Bg2ioSize bg2io_writeByte(Bg2ioBufferIterator *it, const unsigned char in);

/**
 * @brief Write an integer value to a buffer pointed by an iterator, and increment it four bytes. 
 * If necessary, increase the buffer size to fit the added data. In this case, the length and actualLength 
 * attributes of the buffer will be increased.
 * 
 * @param it input buffer iterator
 * @param in input value
 * @return Bg2ioSize Returns the written bytes or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater than the buffer length
 * 
 */
Bg2ioSize bg2io_writeInteger(Bg2ioBufferIterator *it, int in);

/**
 * @brief Write floating point 32 bit value to a buffer pointed by an iterator, and increment it four bytes. 
 * If necessary, increase the buffer size to fit the added data. In this case, the length and actualLength 
 * attributes of the buffer will be increased.
 * 
 * @param it input buffer iterator
 * @param in input value
 * @return Bg2ioSize Returns the written bytes or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater than the buffer length
 * 
 */
Bg2ioSize bg2io_writeFloat(Bg2ioBufferIterator *it, float in);

/**
 * @brief Write a string to the buffer, in Pascal format; thats it, a integer value with the string size,
 * followed by the string characters.
 * 
 * @param it input buffer iterator
 * @param in input value
 * @return Bg2ioSize Returns the written bytes or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater than the buffer length
 * 
 */
Bg2ioSize bg2io_writeString(Bg2ioBufferIterator *it, const char * in);


/**
 * @brief Write a floating point 32 bit array to the buffer. The size of the array, expressed in number
 * of elements, is written, followed by the array values.
 * 
 * @param it input buffer iterator
 * @param in input value
 * @return Bg2ioSize Returns the written bytes or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater than the buffer length
 * 
 */
Bg2ioSize bg2io_writeFloatArray(Bg2ioBufferIterator *it, const float * in);

/**
 * @brief Write an integer 32 bit array to the buffer. The size of the array, expressed in number
 * of elements, is written, followed by the array values.
 * 
 * @param it input buffer iterator
 * @param in input value
 * @return Bg2ioSize Returns the written bytes or:
 *  - BG2IO_ERR_INVALID_PTR if the buffer is invalid
 *  - BG2IO_ERR_ITERATOR_OUT_OF_BOUNDS if the buffer iterator current position
 *    is greater than the buffer length
 * 
 */
Bg2ioSize bg2io_writeIntArray(Bg2ioBufferIterator *it, const int * in);

#endif
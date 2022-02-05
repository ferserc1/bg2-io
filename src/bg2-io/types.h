#ifndef BG2IO_TYPES_H
#define BG2IO_TYPES_H

#include <stdlib.h>

typedef unsigned char Bg2ioByte;
typedef Bg2ioByte * Bg2ioBytePtr;
typedef long long Bg2ioSize;

/**
 * @brief Represents a data buffer
 * 
 * @param mem the memory pointer to the buffer
 * @param length is the size of the buffer that is filled with valid data.
 * @param actualLength is the actual size of the buffer in memory, which may be larger than length.
 * be larger than length
 */
typedef struct Bg2ioBufferT {
    Bg2ioBytePtr mem;
    Bg2ioSize length;
    Bg2ioSize actualLength;
} Bg2ioBuffer;

/**
 * @brief It is important to initialize the Bg2ioBuffer structures before using 
 * them. To do this you can use the following macro, or do it manually.
 * 
 * Bg2ioBuffer b = BG2IO_BUFFER_INIT;
 * 
 * or
 * 
 * Bg2ioBuffer *b = malloc(sizeof(Bg2ioBuffer));
 * BG2IO_BUFFER_PTR_INIT(b);
 * 
 */
#define BG2IO_BUFFER_INIT { NULL, 0L, 0L }
#define BG2IO_BUFFER_PTR_INIT(b) b->mem = NULL; b->length = 0L; b->actualLength = 0L

/**
 * @brief Iterator of a buffer. Used to move through a buffer.
 * 
 * @param buffer pointer to a Buffer structure in memory.
 * @param current current position of the iterator.
 * 
 */
typedef struct Bg2ioBufferIteratorT {
    Bg2ioBuffer * buffer;
    Bg2ioSize current;
} Bg2ioBufferIterator;

/**
 * @brief It is important to initialize the Bg2ioBufferIterator structures before using 
 * them. To do this you can use the following macro, or do it manually.
 * 
 * Bg2ioBuffer b = BG2IO_BUFFER_ITERATOR_INIT;
 * 
 * or
 * 
 * Bg2ioBufferIterator *b = malloc(sizeof(Bg2ioBufferIterator));
 * BG2IO_BUFFER_ITERATOR_PTR_INIT(b);
 * 
 */
#define BG2IO_BUFFER_ITERATOR_INIT { NULL, 0L }

#define BG2IO_ITERATOR(b) { b, 0L }

#endif

#ifndef BG2_IO_BUFFER_MEMORY
#define BG2_IO_BUFFER_MEMORY

#include "types.h"

/**
 * @brief Returns the actual size closest to the required size for a buffer
 * 
 * @param requiredSize 
 * @return Bg2ioSize 
 */
Bg2ioSize bg2io_getActualBufferSize(Bg2ioSize requiredSize);

/**
 * @brief Create a Buffer object. Note that if the supplied buffer
 * contains a valid buffer, its internal pointer will be initialized 
 * and therefore a memory leak will occur.
 * 
 * @param in a pointer to a clear Buffer structure
 * @param requiredSize the required minimum size
 * @return Bg2ioSize the actual size of the buffer, that 
 * can be higher than the require size, -1 if the buffer pointer
 * is NULL or -2 if the internal attributes are not initialized.
 * 
 */
Bg2ioSize bg2io_createBuffer(Bg2ioBuffer *in, Bg2ioSize requiredSize);

/**
 * @brief It ensures that a buffer has sufficient capacity.
 * If it does not have enough capacity, reserve more space.
 * 
 * @param buffer the target buffer.
 * @param requiredSize the required size of the buffer.
 * @return Bg2ioSize returns the actual buffer size, after the
 * operation is executed, -1 if the input buffer is NULL or -2 if
 * the required size is lower than the buffer length (not the actual length).
 */
Bg2ioSize bg2io_reserveBuffer(Bg2ioBuffer *buffer, Bg2ioSize requiredSize);

/**
 * @brief Release a buffer from memory. After calling this function, the internal
 * buffer attribute will be null. This call does not clear the buffer parameter,
 * but clears and initializes its contents.
 * 
 * @param buffer 
 */
void bg2io_freeBuffer(Bg2ioBuffer *buffer);

#endif
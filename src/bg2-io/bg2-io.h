#ifndef BG2_IO
#define BG2_IO

#include "types.h"
#include "buffer-memory.h"
#include "buffer-io.h"

/**
 * @brief Check if the current system is big endian
 * 
 * @return int 1 if is big endian, 0 otherwise
 */
int bg2io_isBigEndian();

/**
 * @brief Check if the current system is little endian
 * 
 * @return int 1 if is little endian, 0 otherwise
 */
int bg2io_isLittleEndian();

#endif

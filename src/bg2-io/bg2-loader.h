
#ifndef BG2_LOADER_H
#define BG2_LOADER_H

#include "types.h"

int bg2io_getParserError(void);

Bg2File * bg2io_parseFileBuffer(Bg2ioBuffer *buffer);



#endif

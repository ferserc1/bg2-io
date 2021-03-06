#!/bin/sh

BG2IO_SRC=../bg2-io/
BG2IO_C_FILES="${BG2IO_SRC}*.c"
WRAPPER_FILES="src/c/wrapper.c src/c/log.c"
C_FILES="$BG2IO_C_FILES $WRAPPER_FILES"

OUTPUT_DIR=build
OUTPUT_LIB=${OUTPUT_DIR}/bg2io.js
OUTPUT_MODULE=${OUTPUT_DIR}/Bg2ioModule.cjs

mkdir -p $OUTPUT_DIR

EXPORTED_FUNCTIONS=_malloc,_free,_loadBg2File,_freeBg2File,_freeString,_freeStringRef,_getBg2FileHeader,_getMaterialStringRef,_getComponentStringRef,_getJointStringRef,_getPolyList,_getStringStructRefFromCStr,_createBg2File,_setFileHeader,_setMaterialData,_setComponentData,_setJointData,_createPolyList,_addPolyList,_addFloatBuffer,_addIndexBuffer,_createPolyListArray,_createBufferWithBg2File,_freeBufferWrapper
EXPORTED_RUNTIME_METHODS=ccall,cwrap,ALLOC_NORMAL,allocate,intArrayFromString,getValue,setValue,stringToUTF8

EXPORT_NAME="'Bg2ioModule'"

emcc -I$BG2IO_SRC $C_FILES -o $OUTPUT_LIB -s MODULARIZE -s EXPORTED_FUNCTIONS=$EXPORTED_FUNCTIONS -s EXPORTED_RUNTIME_METHODS=$EXPORTED_RUNTIME_METHODS -s EXPORT_NAME=$EXPORT_NAME -s ALLOW_MEMORY_GROWTH=1
emcc -I$BG2IO_SRC $C_FILES -o $OUTPUT_MODULE -s ENVIRONMENT=node -s MODULARIZE -s EXPORTED_FUNCTIONS=$EXPORTED_FUNCTIONS -s EXPORTED_RUNTIME_METHODS=$EXPORTED_RUNTIME_METHODS -s EXPORT_NAME=$EXPORT_NAME -s ALLOW_MEMORY_GROWTH=1

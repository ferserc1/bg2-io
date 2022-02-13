#include <emscripten.h>
#include <bg2-io.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

typedef struct StringT {
    int length;
    char * data;
} String;

String * createString(char * c_str, int copy, int debug)
{
    String * result = (String*)malloc(sizeof(String));
    result->length = strlen(c_str);
    if (copy == 0)
    {
        debugLogParam("Creating string reference",c_str,20,debug);
        result->data = c_str;
    }
    else {
        debugLogParam("Creating string copy",c_str,20,debug);
        result->data = (char*)malloc(sizeof(char) * result->length);
        strcpy(result->data, c_str);
    }
    return result;
}

EMSCRIPTEN_KEEPALIVE
String * getStringStructRefFromCStr(char * strPtr, int debug)
{
    return createString(strPtr, 0, debug);
}

EMSCRIPTEN_KEEPALIVE
void freeStringRef(String * s, int debug)
{
    debugLogParam("Releasing string reference", s->data, 20, debug);
    free(s);
}

EMSCRIPTEN_KEEPALIVE
void freeString(String * s, int debug) {
    debugLogParam("Releasing string copy", s->data, 20, debug);
    free(s->data);
    free(s);
}

EMSCRIPTEN_KEEPALIVE
Bg2File * loadBg2File(unsigned char * data, int size, int debug)
{
    debugLogFormat(debug, "Loading bg2 file...");

    Bg2File * file = NULL;
    Bg2ioBuffer fileBuffer = BG2IO_BUFFER_INIT;

    fileBuffer.mem = data;
    fileBuffer.length = (int) size;

    debugLogFormat(debug, "Parsing bg2 file data (%d bytes)", size);
    file = bg2io_parseFileBuffer(&fileBuffer);
    if (file == NULL)
    {
        errorLog("Could not load bg2 file. Parse error");
    }

    return file;
}

EMSCRIPTEN_KEEPALIVE
Bg2FileHeader * getBg2FileHeader(Bg2File * file, int debug)
{
    return &file->header;
}

EMSCRIPTEN_KEEPALIVE
String * getMaterialStringRef(Bg2File * file, int debug)
{
    if (file->materialData != NULL)
    {
        String * result = createString(file->materialData, 0, debug);
        return result;
    }
    else
    {
        debugLog("No material data found in file", debug);
        return NULL;
    }
}

EMSCRIPTEN_KEEPALIVE
String * getComponentStringRef(Bg2File * file, int debug)
{
    if (file->componentData != NULL)
    {
        String * result = createString(file->componentData, 0, debug);
        return result;
    }
    else
    {
        debugLog("No component data found in file", debug);
        return NULL;
    }
}

EMSCRIPTEN_KEEPALIVE
String * getJointStringRef(Bg2File * file, int debug)
{
    if (file->jointData != NULL)
    {
        String * result = createString(file->jointData, 0, debug);
        return result;
    }
    else
    {
        debugLog("No joint data found in file", debug);
        return NULL;
    }
}

EMSCRIPTEN_KEEPALIVE
Bg2ioPolyList * getPolyList(Bg2File * file, int index, int debug)
{
    if (index < file->plists->length)
    {
        return file->plists->data[index];
    }
    else
    {
        errorLogFormat("Error getting polyList: index out of bounds. Poly list count: %d, requested index: %d", file->plists->length, index);
        return NULL;
    }
}

EMSCRIPTEN_KEEPALIVE
void freeBg2File(Bg2File * file, int debug)
{
    debugLog("Releasing bg2 file...", debug);
    bg2io_freeBg2File(file);
}


EMSCRIPTEN_KEEPALIVE
Bg2File * createBg2File(int debug)
{
    debugLog("Creating empty Bg2File struct", debug);
    Bg2File * file = (Bg2File*)malloc(sizeof(Bg2File));
    return file;
}

EMSCRIPTEN_KEEPALIVE
void setFileHeader(
    Bg2File *file, 
    unsigned char endianess, 
    unsigned char majorVersion, 
    unsigned char minorVersion, 
    unsigned char revision, 
    int numberOfPlist,
    int debug)
{
    debugLogFormat(debug, "Setting file header: %s, v:%d.%d.%d, %d polyLists",
        endianess==0 ? "littleEndian" : "bigEndian", (int) majorVersion, (int) minorVersion, (int) revision, numberOfPlist);

    file->header.endianess = endianess;
    file->header.majorVersion = majorVersion;
    file->header.minorVersion = minorVersion;
    file->header.revision = revision;
    file->header.numberOfPolyList = numberOfPlist;
}

EMSCRIPTEN_KEEPALIVE
void setMaterialData(Bg2File *file, const char * matText, int debug)
{
    char * mat = (char*)malloc(strlen(matText) + 1);
    strcpy(mat, matText);
    debugLogParam("Setting material data:", mat, 30, debug);
    file->materialData = mat;
}

EMSCRIPTEN_KEEPALIVE
void setComponentData(Bg2File *file, const char * compText, int debug)
{
    char * comp = (char*)malloc(strlen(compText) + 1);
    strcpy(comp, compText);
    debugLogParam("Setting component data:", comp, 30, debug);
    file->componentData = comp;
}

EMSCRIPTEN_KEEPALIVE
void setJointData(Bg2File *file, const char * jointText, int debug)
{
    char * joint = (char*)malloc(strlen(jointText) + 1);
    strcpy(joint, jointText);
    debugLogParam("Setting joint data:", joint, 30, debug);
    file->jointData = joint;
}

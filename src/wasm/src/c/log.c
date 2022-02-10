

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "log.h"

void debugLog(char * text, int debug)
{
    if (debug != 0)
    {
        
        printf("WASM debug log: %s\n", text);
    }
}

void debugLogParam(char * text, char * param, int maxLength, int debug)
{
    if (debug != 0)
    {
        if (maxLength>0 && strlen(param)>maxLength)
        {
            param[maxLength - 3] = '.';
            param[maxLength - 2] = '.';
            param[maxLength - 1] = '.';
            param[maxLength] = '\0';
        }

        printf("WASM debug log: %s - %s\n", text, param);
    }
}

void debugLogFormat(int debug, char * text, ...)
{
    if (debug != 0)
    {
        char buffer[4096];
        va_list args;
        va_start(args, text);
        int rc = vsnprintf(buffer, sizeof(buffer), text, args);
        va_end(args);
        printf("WASM debug log: %s\n", buffer);
    }
}

void errorLog(char * text)
{
    printf("WASM ERROR: %s\n", text);
}

void errorLogParam(char * text, char * param, int maxLength)
{
    if (maxLength>0 && strlen(param)>maxLength)
    {
        param[maxLength - 3] = '.';
        param[maxLength - 2] = '.';
        param[maxLength - 1] = '.';
        param[maxLength] = '\0';
    }

    printf("WASM ERROR: %s - %s\n", text, param);
}

void errorLogFormat(char * text, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, text);
    int rc = vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);
    printf("WASM ERROR: %s\n", buffer);
}

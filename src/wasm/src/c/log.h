#ifndef BG2_WASM_LOG
#define BG2_WASM_LOG

void debugLog(char * text, int debug);
void debugLogParam(char * text, char * param, int maxLength, int debug);
void debugLogFormat(int debug, char * text, ...);

void errorLog(char * text);
void errorLogParam(char * text, char * param, int maxLength);
void errorLogFormat(char * text, ...);

#endif

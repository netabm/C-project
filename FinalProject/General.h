#ifndef _GENERAL_
#define _GENERAL_

#define MAX_STR_LEN 255
typedef enum { False, True }BOOL;

void getInt(int* num, char* msg);
char* getStrExactName(const char* msg);
char* getDynStr(char* str);
char* myGets(char* buffer, int size, FILE* source);
void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));

#endif
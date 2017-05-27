#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#pragma warning(disable:4244) 
#pragma warning(disable:4800) 
#endif

void SetMediaPath(const char path[]);
const char * FullPath(const char filename[]);
char * MakeFullPath(const char dir[], const char filename[], char fullpath[]);

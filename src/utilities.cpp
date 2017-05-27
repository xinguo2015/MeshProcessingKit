#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilities.h"



char gMediaPath[256] = "";

void SetMediaPath(const char path[])
{
	strcpy(gMediaPath, path);
}

char * MakeFullPath(const char dir[], const char filename[], char fullpath[])
{
	strcpy(fullpath, dir);
	return strcat(fullpath, filename);
}

const char * FullPath(const char filename[])
{
	static char path[256];
	return MakeFullPath(gMediaPath, filename, path);
}


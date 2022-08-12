#include "ftpserver.h"

int changeDirectory(char *path)
{
    if (path[strlen(path)] == '\n')
    {
        path[strlen(path)] = '\0';
    }
    return chdir(path);
}

int changeDirectoryUp()
{
    char currentDirectory[MAX_SIZE];
    return chdir("..");
}
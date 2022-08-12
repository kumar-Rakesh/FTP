#include "ftpserver.h"

int del(char *path)
{
    if (access(path, F_OK) == -1)
    {
        return -1;
    }
    return remove(path);
}
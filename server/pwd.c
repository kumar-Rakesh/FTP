#include "ftpserver.h"

void pwd(char *directory)
{
    getcwd(directory, MAX_SIZE);
}
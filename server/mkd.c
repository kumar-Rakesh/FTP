#include "ftpserver.h"

int mkd(char *directoryName)
{
    return mkdir(directoryName, S_IRWXU);
}
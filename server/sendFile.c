#include "ftpserver.h"

int sendFile(char *fileName, int data)
{
    char fileData[MAX_SIZE];
    bzero(fileData, sizeof(fileData));
    
    int fd;
    if ((fd = open(fileName, O_RDONLY)) < 0)
    {
        return -1;
    }
    char buf;
    int i = 0;
    while (read(fd, &buf, 1) > 0)
    {
        fileData[i++] = buf;
    }
    send(data, fileData, MAX_SIZE, 0);
    close(fd);
    return 0;
}
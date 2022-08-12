#include "ftpserver.h"

int recvFile(char *fileName, int dataSock)
{

    char fileData[MAX_SIZE];
    bzero(fileData, sizeof(fileData));

    recv(dataSock, fileData, MAX_SIZE, 0);

    int fd;
    if ((fd = open(fileName, O_CREAT | O_WRONLY, 0777)) < 0)
    {
        return -1;
    }
    write(fd, fileData, MAX_SIZE);
    close(fd);
    return 0;
}
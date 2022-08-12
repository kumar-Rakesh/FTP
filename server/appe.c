#include "ftpserver.h"

int append(char *path, int dataSock)
{
    int fd;
    int fileExists = access(path, F_OK);
    if (fileExists == -1)
    {
        if ((fd = open(path, O_WRONLY | O_CREAT, 0777)) < 0)
        {
            return -1;
        }
    }
    else
    {
        if ((fd = open(path, O_APPEND)) < 0)
        {
            return -1;
        }
    }
    char buffer[MAX_SIZE];

    recv(dataSock, buffer, MAX_SIZE, 0);

    write(fd, buffer, MAX_SIZE);

    close(fd);

    return 0;
}
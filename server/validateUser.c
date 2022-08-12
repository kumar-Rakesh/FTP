#include "ftpserver.h"

char user[5][7] = {"rakesh", "kumar", "abcde", "ria", "test"};

int isUserCommand(char buffer[], char args[])
{
    if (strncmp(buffer, "USER ", 5) == 0 ||
        strncmp(buffer, "user ", 5) == 0 ||
        strncmp(buffer, "USER\n", 5) == 0 ||
        strncmp(buffer, "user\n", 5) == 0)
    {
        strcpy(args, buffer + 5);
        args[strlen(args) - 1] = '\0';
        return 1;
    }
    return 0;
}

int validateUser(char buffer[MAX_SIZE])
{
    char args[MAX_SIZE];
    if (!isUserCommand(buffer, args))
    {
        return -1;
    }
    if (strlen(args) == 0)
    {
        return 2;
    }
    for (int i = 0; i < 5; ++i)
    {
        if (strncmp(user[i], args, strlen(user[i])) == 0)
        {
            return 1;
        }
    }
    return 3;
}
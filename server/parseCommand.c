#include "ftpserver.h"

void parseCommand(char buffer[], int *userLoggedIn, int *command, char args[])
{
    if (*userLoggedIn == 0)
    {
        if (strncmp(buffer, "user ", 5) == 0 || strncmp(buffer, "USER ", 5) == 0)
        {
            strcpy(args, buffer + 5);
            args[strlen(args) - 1] = '\0';
            *command = USER;
        }
    }
}
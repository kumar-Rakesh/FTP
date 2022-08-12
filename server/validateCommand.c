#include "ftpserver.h"

int validateCommand(char command[], char args[])
{
    if (strncmp(command, "RETR ", 5) == 0 ||
        strncmp(command, "retr ", 5) == 0 ||
        strncmp(command, "RETR\n", 5) == 0 ||
        strncmp(command, "retr\n", 5) == 0)
    {
        strcpy(args, command + 5);
        args[strlen(args) - 1] = '\0';
        return RETR;
    }
    else if (strncmp(command, "STOR ", 5) == 0 ||
             strncmp(command, "stor ", 5) == 0 ||
             strncmp(command, "STOR\n", 5) == 0 ||
             strncmp(command, "stor\n", 5) == 0)
    {
        strcpy(args, command + 5);
        args[strlen(args) - 1] = '\0';
        return STOR;
    }
    else if (strncmp(command, "LIST ", 5) == 0 ||
             strncmp(command, "list ", 5) == 0 ||
             strncmp(command, "LIST\n", 5) == 0 ||
             strncmp(command, "list\n", 5) == 0)
    {
        return LIST;
    }
    else if (strncmp(command, "QUIT ", 5) == 0 ||
             strncmp(command, "quit ", 5) == 0 ||
             strncmp(command, "QUIT\n", 5) == 0 ||
             strncmp(command, "quit\n", 5) == 0)
    {
        return QUIT;
    }
    else if (strncmp(command, "PWD ", 4) == 0 ||
             strncmp(command, "pwd ", 4) == 0 ||
             strncmp(command, "PWD\n", 4) == 0 ||
             strncmp(command, "pwd\n", 4) == 0)
    {
        return PWD;
    }
    else if (strncmp(command, "MKD ", 4) == 0 ||
             strncmp(command, "mkd ", 4) == 0 ||
             strncmp(command, "MKD\n", 4) == 0 ||
             strncmp(command, "mkd\n", 4) == 0)
    {
        strcpy(args, command + 4);
        args[strlen(args) - 1] = '\0';
        return MKD;
    }
    else if (strncmp(command, "CDUP ", 5) == 0 ||
             strncmp(command, "cdup ", 5) == 0 ||
             strncmp(command, "CDUP\n", 5) == 0 ||
             strncmp(command, "cdup\n", 5) == 0)
    {
        return CDUP;
    }
    else if (strncmp(command, "CWD ", 4) == 0 ||
             strncmp(command, "cwd ", 4) == 0 ||
             strncmp(command, "CWD\n", 4) == 0 ||
             strncmp(command, "cwd\n", 4) == 0)
    {
        return CWD;
    }
    else if (strncmp(command, "RMD ", 4) == 0 ||
             strncmp(command, "rmd ", 4) == 0 ||
             strncmp(command, "RMD\n", 4) == 0 ||
             strncmp(command, "rmd\n", 4) == 0)
    {
        strcpy(args, command + 4);
        args[strlen(args) - 1] = '\0';
        return RMD;
    }
    else if (strncmp(command, "DELE ", 5) == 0 ||
             strncmp(command, "dele ", 5) == 0 ||
             strncmp(command, "DELE\n", 5) == 0 ||
             strncmp(command, "dele\n", 5) == 0)
    {
        strcpy(args, command + 5);
        args[strlen(args) - 1] = '\0';
        return DELE;
    }
    else if (strncmp(command, "NOOP ", 5) == 0 ||
             strncmp(command, "noop ", 5) == 0 ||
             strncmp(command, "NOOP\n", 5) == 0 ||
             strncmp(command, "noop\n", 5) == 0)
    {
        return NOOP;
    }
    else if (strncmp(command, "APPE ", 5) == 0 ||
             strncmp(command, "appe ", 5) == 0 ||
             strncmp(command, "APPE\n", 5) == 0 ||
             strncmp(command, "appe\n", 5) == 0)
    {
        strcpy(args, command + 5);
        args[strlen(args) - 1] = '\0';
        return APPE;
    }
    return INVALID;
}
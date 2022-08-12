#include "ftpserver.h"

int list(char *entries)
{
    DIR *directory;
    struct dirent *entry;
    entries = '\0';

    if ((directory = opendir("./")) == NULL)
    {
        return -1;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
        {
            continue;
        }
        else
        {
            strcat(entries, entry->d_name);
            strcat(entries, "\n");
        }
    }
    closedir(directory);
    return 0;
}
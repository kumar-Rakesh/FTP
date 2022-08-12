#include "ftpserver.h"

int rmd(char *path)
{
    DIR *directory;
    struct dirent *entry;

    if ((directory = opendir(path)) == NULL)
    {
        return -1;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (opendir(entry->d_name) == NULL)
        {
            remove(entry->d_name);
        }
        else
        {
            char newPath[MAX_SIZE];
            strcat(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, entry->d_name);
            rmd(newPath);
            rmdir(entry->d_name);
        }
    }
    return 0;
}
#include "ftpserver.h"

void sendMenu(int socket)
{
    char buffer[MAX_SIZE];
    bzero(buffer, sizeof(bzero));
    strcpy(buffer, "\n");
    strcat(buffer, "\n\tFTP\t\n");
    strcat(buffer, "\nMenu\n");
    strcat(buffer, "\n 1.\t USER ---- usage: USER <username>\n");
    send(socket, buffer, MAX_SIZE, 0);
}
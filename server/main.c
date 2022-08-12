#include "ftpserver.h"
#include "sendMenu.c"
#include "parseCommand.c"
#include "validateUser.c"
#include "validateCommand.c"
#include "list.c"
#include "sendFile.c"
#include "recvFile.c"
#include "pwd.c"
#include "mkd.c"
#include "changeDirectory.c"
#include "rmd.c"
#include "del.c"
#include "appe.c"

void sendToClient(int client, char msg[])
{
    char buffer[MAX_SIZE];
    bzero(buffer, sizeof(buffer));
    strcpy(buffer, msg);
    send(client, buffer, MAX_SIZE, 0);
}

int createSocket()
{
    int s;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket\n");
        exit(1);
    }
    return s;
}

void addAddressInfo(struct sockaddr_in *servAdd, int portNumber)
{
    servAdd->sin_family = AF_INET;
    servAdd->sin_addr.s_addr = INADDR_ANY;
    servAdd->sin_port = htons((uint16_t)portNumber);
}

void bindSocketAndAddress(int sock, struct sockaddr_in *servAdd)
{
    if (bind(sock, (struct sockaddr *)servAdd, sizeof(*servAdd)) == 0)
    {
        printf("Listening!!");
    }
    else
    {
        printf("Could not bind the server!!\n");
        exit(1);
    }
}

int main()
{
    int PORTNO = 5001;
    int client, sd, dataPortNumber, data;
    socklen_t len;
    struct sockaddr_in servAdd, clientAddr, dataAddr;
    int clientLen = sizeof(clientAddr);

    sd = createSocket();

    printf("Server Socket created successfully!!\n");

    addAddressInfo(&servAdd, PORTNO);

    int val = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));

    bindSocketAndAddress(sd, &servAdd);

    listen(sd, 5);

    char buffer[MAX_SIZE];

    while (1)
    {
        client = accept(sd, (struct sockaddr *)&clientAddr, &clientLen);
        printf("Accepted the client\n");

        if (fork() == 0)
        {

            sendMenu(client);
            bzero(buffer, sizeof(buffer));
            printf("Waiting for USER command\n");
            int userLoggedIn = 0, portDecided = 0;
            while (1)
            {
                bzero(buffer, sizeof(buffer));
                recv(client, buffer, MAX_SIZE, 0);
                int valUser = validateUser(buffer);
                if (valUser == 1)
                {
                    break;
                }
                else if (valUser == 2)
                {
                    sendToClient(client, "Username is missing!!");
                }
                else if (valUser == 3)
                {
                    sendToClient(client, "Username is invalid");
                }
                else
                {
                    sendToClient(client, "Kindly login with USER command first!!");
                }
            }
            userLoggedIn = 1;
            bzero(buffer, sizeof(buffer));
            strcpy(buffer, "You are logged in!");
            send(client, buffer, MAX_SIZE, 0);
            while (1)
            {
                bzero(buffer, sizeof(buffer));
                recv(client, buffer, MAX_SIZE, 0);
                char command[MAX_SIZE];
                bzero(command, sizeof(command));
                strcpy(command, buffer);
                printf("Command Entered: %s\n", command);
                char args[MAX_SIZE];
                int commandIndex = validateCommand(command, args);
                if (commandIndex == -1)
                {
                    bzero(buffer, sizeof(bzero));
                    strcpy(buffer, "Command is invalid!");
                    send(client, buffer, MAX_SIZE, 0);
                }
                else if (commandIndex == PORT)
                {
                    data = createSocket();
                    sscanf(args, "%d", &dataPortNumber);
                    addAddressInfo(&dataAddr, dataPortNumber);
                    bindSocketAndAddress(data, &dataAddr);
                    portDecided = 1;
                    printf("Data Channel established!!");
                    bzero(buffer, sizeof(buffer));
                    strcpy(buffer, "Data Channel is established!!");
                    send(client, buffer, MAX_SIZE, 0);
                }
                else if (commandIndex == QUIT)
                {
                    if (data > 0)
                    {
                        close(data);
                    }
                    close(client);
                    exit(0);
                }
                else if (commandIndex == LIST)
                {
                    if (portDecided == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly send PORT first!!");
                        continue;
                    }
                    char entries[MAX_SIZE];
                    if (list(entries) == -1)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Operation not successful!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        send(data, entries, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == RETR)
                {
                    if (portDecided == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly send PORT first!!");
                        continue;
                    }
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter file name too!!");
                        continue;
                    }
                    int res = sendFile(args, data);
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Either file name is missing or file name not present!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("File sent successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "File sent successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == STOR)
                {
                    if (portDecided == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly send PORT first!!");
                        continue;
                    }
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter file name too!!");
                        continue;
                    }
                    int res = recvFile(args, data);
                    if (res == -1)
                    {

                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Unable to create file!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("File stored successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "File stored successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == PWD)
                {
                    char path[MAX_SIZE];
                    bzero(path, sizeof(path));
                    pwd(path);
                    if (strlen(path) == 0)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Unable to get current working directory!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        send(client, path, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == MKD)
                {
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter directory name too!!");
                        continue;
                    }
                    int res = mkd(args);
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Unable to create directory!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("Directory created successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Directory created successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == CDUP)
                {
                    int res = changeDirectoryUp();
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Unable to change directory!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("Directory changed successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Directory changed successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == CWD)
                {
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter directory path too!!");
                        continue;
                    }
                    int res = changeDirectory(args);
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Unable to change directory!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("Directory changed successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Directory changed successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == RMD)
                {
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter directory name too!!");
                        continue;
                    }
                    char directory[MAX_SIZE];
                    getcwd(directory, MAX_SIZE);
                    strcat(directory, "/");
                    strcat(directory, args);
                    int res = rmd(directory);
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Directory doesn't exist!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("Directory deleted successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Directory deleted successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == DELE)
                {
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter directory name too!!");
                        continue;
                    }
                    char absPath[MAX_SIZE];
                    getcwd(absPath, MAX_SIZE);
                    strcat(absPath, "/");
                    strcat(absPath, args);
                    int res = del(absPath);
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! File doesn't exist!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("File deleted successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "File deleted successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
                else if (commandIndex == NOOP)
                {
                    printf("NOOP acknowledged!!");
                    bzero(buffer, sizeof(bzero));
                    strcpy(buffer, "NOOP acknowledged!!");
                    send(client, buffer, MAX_SIZE, 0);
                }
                else if (commandIndex == APPE)
                {
                    if (strlen(args) == 0)
                    {
                        bzero(buffer, sizeof(buffer));
                        strcpy(buffer, "Kindly enter file name too!!");
                        continue;
                    }
                    char absPath[MAX_SIZE];
                    getcwd(absPath, MAX_SIZE);
                    strcat(absPath, "/");
                    strcat(absPath, args);
                    int res = append(absPath, data);
                    if (res == -1)
                    {
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Operation can't be completed!! Error in opening file!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                    else
                    {
                        printf("Data appended to file successfully!!");
                        bzero(buffer, sizeof(bzero));
                        strcpy(buffer, "Data appended to file successfully!!");
                        send(client, buffer, MAX_SIZE, 0);
                    }
                }
            }
        }
    }
}
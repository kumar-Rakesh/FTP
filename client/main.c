#include "ftpclient.h"

int main()
{
    int PORTNO = 5001;
    int client, sd, portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket\n");
        exit(1);
    }

    printf("Client Socket created successfully!!\n");

    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAdd.sin_port = htons((uint16_t)PORTNO);

    if (connect(sd, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
    {
        printf("Connection with server failed!!\n");
        exit(1);
    }
    printf("Connection with server established!!\n");

    char buffer[MAX_SIZE];

    recv(sd, buffer, MAX_SIZE, 0);

    printf("%s\n", buffer);

    while (1)
    {

        bzero(buffer, sizeof(buffer));

        printf("Please enter command:\t");

        fgets(buffer, MAX_SIZE, stdin);

        send(sd, buffer, MAX_SIZE, 0);

        bzero(buffer, sizeof(buffer));

        recv(sd, buffer, MAX_SIZE, 0);

        printf("%s\n", buffer);
    }
}
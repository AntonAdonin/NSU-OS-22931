#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <ctype.h>

#define SOCKET_PATH "./socket31"

void handle_client(int clientVar) 
{
    char buffer[256];
    ssize_t bytesCount;

    bytesCount = recv(clientVar, buffer, sizeof(buffer), 0);

    if (bytesCount < 0) 
    {
        perror("Error receiving data from client");
        exit(-1);
    } 

    else if (bytesCount == 0) 
    {
        close(clientVar);
        return;
    }

    for (ssize_t i = 0; i < bytesCount; ++i) 
    {
        buffer[i] = toupper(buffer[i]);
    }

    printf("Client %d said: %s\n", clientVar, buffer);
}

int main()
{
    int serverVar, clientVar;
    struct sockaddr_un serverAddress, clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    serverVar = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverVar < 0) 
    {
        perror("Creating error");
        exit(-1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, SOCKET_PATH, sizeof(serverAddress.sun_path) - 1);

    if (bind(serverVar, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
    {
        perror("Bind error");
        exit(-1);
    }

    if (listen(serverVar, 5) < 0) {
        perror("Listening error");
        exit(-1);
    }

    printf("Listening %s\n", SOCKET_PATH);

    fd_set read_fds;
    int max_fd;

    while (1) 
    {
        FD_ZERO(&read_fds);
        FD_SET(serverVar, &read_fds);
        max_fd = serverVar;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) 
        {
            perror("Select error");
            exit(-1);
        }

        if (FD_ISSET(serverVar, &read_fds)) 
        {
            clientVar = accept(serverVar, (struct sockaddr*)&clientAddress, &clientAddressLen);
            if (clientVar < 0) 
            {
                perror("Accepting error");
                exit(-1);
            }

            FD_SET(clientVar, &read_fds);
            if (clientVar > max_fd) 
            {
                max_fd = clientVar;
            }

            printf("New connection from %d\n", clientVar);
        }

        for (int i = serverVar + 1; i <= max_fd; ++i) 
        {
            if (FD_ISSET(i, &read_fds)) 
            {
                handle_client(i);
            }
        }
    }

    return 0;
}
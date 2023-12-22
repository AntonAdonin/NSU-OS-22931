#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "./socket32"

void* handle_client(void* arg) 
{
    int clientVar = *((int*)arg);
    free(arg);

    char buffer[256];
    ssize_t bytes_received;

    bytes_received = recv(clientVar, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) 
    {
        perror("Recieving error");
    } 

    else if (bytes_received == 0) 
    {
        return NULL;
    }

    for (ssize_t i = 0; i < bytes_received; ++i) 
    {
        buffer[i] = toupper(buffer[i]);
    }

    printf("Received from %d: %s\n", clientVar, buffer);

    return NULL;
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

    if (bind(serverVar, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Binding error");
        exit(-1);
    }

    if (listen(serverVar, 5) < 0) {
        perror("Listening error");
        exit(-1);
    }

    printf("Listening %s\n", SOCKET_PATH);

    while (1) 
    {
        clientVar = accept(serverVar, (struct sockaddr*)&clientAddress, &clientAddressLen);
        if (clientVar < 0) 
        {
            perror("Accepting error");
            exit(-1);
        }

        int* arg = malloc(sizeof(int));
        *arg = clientVar;
        pthread_t thread;

        if (pthread_create(&thread, NULL, handle_client, arg) != 0) 
        {
            perror("Creating error");
            close(clientVar);
            free(arg);
        }
    }

    return 0;
}
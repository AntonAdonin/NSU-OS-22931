#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "./socket32"

int main() 
{
    int socketVar;
    struct sockaddr_un serverAddress;

    socketVar = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketVar < 0) 
    {
        perror("Creating error");
        exit(-1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, SOCKET_PATH, sizeof(serverAddress.sun_path) - 1);

    if (connect(socketVar, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
    {
        perror("Connecting error");
        exit(-1);
    }

    char message[256];
    read(STDIN_FILENO, message, sizeof(message));
    if (send(socketVar, message, sizeof(message), 0) < 0) 
    {
        perror("Sending error");
        exit(-1);
    }

    return 0;
}
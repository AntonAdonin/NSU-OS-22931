#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"

int main() {
    // Creating a soket
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("Error of creation a soket");
        exit(EXIT_FAILURE);
    }

    // Initializing a server's adress structure
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Binding a soket to the adress
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) 
    {
        perror("Error of binding a soket");
        exit(EXIT_FAILURE);
    }

    // Waiting for client's connection
    if (listen(sockfd, 1) == -1)
    {
        perror("Error of waiting a client's connection");
        exit(EXIT_FAILURE);
    }

    // Accepting client's connections
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1)
    {
        perror("Error of accepting a client's connection");
        exit(EXIT_FAILURE);
    }

    // Processing the messages
    char buffer[256];
    ssize_t bytesRead;
    while ((bytesRead = read(clientfd, buffer, sizeof(buffer))) > 0) 
    {
        for (int i = 0; i < bytesRead; i++)
            buffer[i] = toupper(buffer[i]);
        printf("%s", buffer);
    }

    // Closing sokets
    close(clientfd);
    close(sockfd);

    return 0;
}
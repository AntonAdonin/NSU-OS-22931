#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

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

    // Trying to make a connection with the server
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1) 
    {
        perror("Error of connection with the server");
        exit(EXIT_FAILURE);
    }

    // Sending a message
    char message[] = "Meow!";
    if (write(sockfd, message, strlen(message)) == -1) 
    {
        perror("Error of sending the message");
        exit(EXIT_FAILURE);
    }

    // Closing a soket
    close(sockfd);

    return 0;
}
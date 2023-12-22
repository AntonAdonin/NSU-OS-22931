#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


int main() {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

//    unlink("/socket");

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket");

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connection...\n");

    int client_sockfd = accept(sockfd, NULL, NULL);

    if (client_sockfd == -1) {
        perror("accept");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    char buf;

    while (read(client_sockfd, &buf, 1) != 0) {
        printf("%c", toupper(buf));
    }
    printf("\n");

    close(client_sockfd);
    close(sockfd);

    unlink("socket");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

int main() {
    char message[100];


    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected\n");

    for(int i =0; i<3;i++){
        scanf("%s", message);
        // const char *message = "Hello, server!\0";
        if (write(sockfd, message, strlen(message)) == -1) {
            perror("write");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

    }

    close(sockfd);

    return 0;
}
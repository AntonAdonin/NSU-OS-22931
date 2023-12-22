#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define PATH "./socket"

int main() {
    int client_fd;
    struct sockaddr_un server_addr;

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket failed");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, PATH);

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(1);
    }

    char buffer[256];
    printf("Write your message: ");
    scanf("%s", buffer);
    write(client_fd, buffer, strlen(buffer));
    read(client_fd, buffer, strlen(buffer));
    printf("Server message: %s", buffer);


    close(client_fd);
    return 0;
}
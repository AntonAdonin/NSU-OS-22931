#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <ctype.h>

#define PATH "./socket"
#define MAX_CLIENTS 5

int main() {
    unlink(PATH);

    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[256];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    fd_set master_fds, read_fds;
    FD_ZERO(&master_fds);
    FD_SET(server_fd, &master_fds);
    int max_fd = server_fd + 1;

    printf("Server is listening...\n");

    while (1) {
        read_fds = master_fds;

        if (select(max_fd, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error in select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_fd, &read_fds)) {
            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

            FD_SET(client_fd, &master_fds);
            if (client_fd >= max_fd) {
                max_fd = client_fd + 1;
            }

            printf("Client %d connected\n", client_fd);
        }

        for (int fd = server_fd + 1; fd < max_fd; fd++) {
            if (FD_ISSET(fd, &read_fds)) {
                ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    for (int i = 0; i < bytesRead; i++) {
                        buffer[i] = toupper(buffer[i]);
                    }
                    write(fd, "SUCESS", 7);
                    printf("Client %d message: %s\n", fd, buffer);
                } else if (bytesRead == 0) {
                    printf("Client %d disconnected\n", fd);
                    close(fd);
                    FD_CLR(fd, &master_fds);
                } else {
                    perror("Error reading from client");
                }
            }
        }
    }

    close(server_fd);
    unlink(PATH);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define PATH "/tmp/socket"

int main() {
    
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, PATH);
    
    unlink(PATH);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(1);
    }


    if (listen(server_fd, 1) == -1) {
        printf("listen failed\n");
    }

    printf("Server is listening...\n");

    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept failed");
        exit(1);
    }
    
    char buffer[256];
    int readed_bytes = read(client_fd, buffer, sizeof(buffer));
    int i;
    for (i = 0; i < readed_bytes; i++){
        putchar(toupper(buffer[i]));
    }

    close(client_fd);
    close(server_fd);

    unlink(PATH);

    return 0;
}

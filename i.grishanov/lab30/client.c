#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket_example"

int main() {
    int client_socket;
    struct sockaddr_un server_address;

    // create Unix domain socket
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // set server parametrs 
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);

    // connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("server connection error");
        exit(EXIT_FAILURE);
    }

    // send data to server
    char text[] = "Hello, Server!";
    printf("send to server: %s\n", text);
    send(client_socket, text, strlen(text), 0);

    // get data from server 
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';

    printf("recived from server: %s\n", buffer);

    // close the conection 
    close(client_socket);

    return 0;
}
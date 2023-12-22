#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <pthread.h>

#define PATH "./socket"
#define MAX_CLIENTS 5

void *handle_connection(void *arg)
{
    int client_fd = *(int *)arg;
    char buffer[256];

    ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        for (int j = 0; j < bytes_received; ++j)
        {
            buffer[j] = toupper(buffer[j]);
        }
        printf("Client %d message: %s\n", client_fd, buffer);
        write(client_fd, "SUCESS", 7);
    }
    else
    {
        close(client_fd);
        printf("Connection closed for client %d\n", client_fd);
        write(client_fd, "FAIL", 5);
    }

    free(arg);
    pthread_exit(NULL);
}

int main()
{

    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, PATH);

    unlink(PATH);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind failed");
        exit(1);
    }

    if (listen(server_fd, MAX_CLIENTS) == -1)
    {
        printf("listen failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");

    client_len = sizeof(client_addr);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1)
        {
            perror("accept failed");
            exit(1);
        }

        printf("Accepted new connection\n");

        int* data = (int*)malloc(sizeof(int));
        *data = client_fd;

        pthread_t thread_id;

        if (pthread_create(&thread_id, NULL, handle_connection, data) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }

        pthread_detach(thread_id);
    }

    close(server_fd);

    unlink(PATH);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"

int main() {
    int client_fd;
    struct sockaddr_un server_addr;

    // Создание сокета
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Установка соединения с сервером
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Отправка текста на сервер
    char message[1024];
    while (1) {
        printf("Enter text: ");
        fgets(message, 1024, stdin);
        send(client_fd, message, strlen(message), 0);
	if (strcmp(message, "EXIT\n") == 0) break;
    }
    perror("send");	
    exit(EXIT_FAILURE);

    // Закрытие сокета
    close(client_fd);

    return 0;
}

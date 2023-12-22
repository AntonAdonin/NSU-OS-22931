#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buf[1025];

    // Создание сокета
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Прослушивание сокета
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Принятие соединения от клиента
    client_len = sizeof(struct sockaddr_un);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Чтение данных от клиента
    if (read(client_fd, buf, sizeof(buf)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Преобразование текста в верхний регистр
    for (int i = 0; i < strlen(buf); i++) {
        buf[i] = toupper(buf[i]);
    }

    // Вывод преобразованного текста
    printf("Received from client: ", buf);

    // Закрытие сокетов
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}

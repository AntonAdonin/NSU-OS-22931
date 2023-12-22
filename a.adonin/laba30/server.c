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
    char buffer[1024];

    // Создание сокета
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязка сокета к адресу
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Ошибка при привязке сокета");
        exit(EXIT_FAILURE);
    }

    // Ожидание подключения клиента
    if (listen(server_fd, 1) == -1) {
        perror("Ошибка при ожидании подключения клиента");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен и ожидает подключения клиента...\n");

    // Принятие подключения от клиента
    client_len = sizeof(struct sockaddr_un);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("Ошибка при принятии подключения от клиента");
        exit(EXIT_FAILURE);
    }

    printf("Клиент подключен\n");

    // Чтение текста от клиента
    memset(buffer, 0, sizeof(buffer));
    if (read(client_fd, buffer, sizeof(buffer)) == -1) {
        perror("Ошибка при чтении данных от клиента");
        exit(EXIT_FAILURE);
    }

    // Преобразование текста в верхний регистр
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            buffer[i] = buffer[i] - 32;
        }
    }

    // Вывод текста в стандартный поток вывода
    printf("Преобразованный текст: %s\n", buffer);

    // Закрытие сокетов
    close(client_fd);
    close(server_fd);

    printf("Сервер завершен\n");

    return 0;
}

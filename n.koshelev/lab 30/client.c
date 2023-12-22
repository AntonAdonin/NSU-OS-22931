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
    char buffer[1024];

    // Создание сокета
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Установка соединения с сервером
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Ошибка при установке соединения с сервером");
        exit(EXIT_FAILURE);
    }

    printf("Соединение с сервером установлено\n");

    // Ввод текста
    printf("Введите текст: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Отправка текста серверу
    if (write(client_fd, buffer, strlen(buffer)) == -1) {
        perror("Ошибка при отправке данных серверу");
        exit(EXIT_FAILURE);
    }

    // Закрытие сокета
    close(client_fd);

    printf("Клиент завершен\n");

    return 0;
}

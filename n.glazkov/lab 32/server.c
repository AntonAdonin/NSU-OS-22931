#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>

#define SOCKET_PATH "/tmp/socket"
#define MAX_EVENTS 10

int main() {
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_un server_addr, client_addr;
    struct epoll_event event, events[MAX_EVENTS];
    char buffer[1024];

    // Создание сокета
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
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

    // Создание экземпляра epoll
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Добавление серверного сокета в экземпляр epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while ((strcmp (buffer, "EXIT\n")) != 0) {
        int n, i;

        // Ожидание событий в экземпляре epoll
        n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // Обработка полученных событий
        for (i = 0; i < n; i++) {
            if (events[i].data.fd == server_fd) {
                // Принятие входящего соединения
                socklen_t client_len = sizeof(struct sockaddr_un);
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_fd == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                // Добавление клиентского сокета в экземпляр epoll
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("epoll_ctl");
                    exit(EXIT_FAILURE);
                }
            } else {
                // Чтение данных от клиента
                ssize_t bytes_read = read(events[i].data.fd, buffer, sizeof(buffer));
                if (bytes_read <= 0) {
                    // Закрытие соединения при ошибке или завершении клиентом
                    close(events[i].data.fd);
                } else {
                    // Преобразование текста в верхний регистр и вывод в стандартный поток вывода
                    for (int j = 0; j < bytes_read; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    printf("%s", buffer);
                }
            }
        }
	if ((strcmp (buffer, "EXIT\n")) == 0) break;
    }

    // Закрытие сокета
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}

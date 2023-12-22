#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>


#define MAX_CLIENTS 10

struct client_info {
    int sockfd;
};

int main() {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    listen(sockfd, SOMAXCONN);

    printf("Waiting for connection...\n");

    struct client_info clients[MAX_CLIENTS];
    fd_set read_fds, active_fds;

    FD_ZERO(&active_fds);
    FD_SET(sockfd, &active_fds);

    while (1) {
        read_fds = active_fds;

        if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        for (int fd = 0; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &read_fds)) {
                if (fd == sockfd) {
                    // Новое подключение
                    int client_sockfd = accept(sockfd, NULL, NULL);
                    if (client_sockfd == -1) {
                        perror("accept");
                        break;
                    }

                    printf("Client connected (fd = %d)!\n", client_sockfd);

                    FD_SET(client_sockfd, &active_fds);

                    int i;
                    for (i = 0; i < MAX_CLIENTS; ++i) {
                        if (clients[i].sockfd == 0) {
                            clients[i].sockfd = client_sockfd;
                            break;
                        }
                    }

                    if (i == MAX_CLIENTS) {
                        fprintf(stderr, "More than Max Clients.\n");
                        close(client_sockfd);
                    }
                }
                else {
                    // Данные от клиента
                    char buffer[1024];
                    int bytes_received = recv(fd, buffer, sizeof(buffer), 0);
                    if (bytes_received <= 0) {
                        // Разрыв соединения или ошибка
                        printf("Client disconnected(fd = %d)!\n", fd);
                        close(fd);
                        FD_CLR(fd, &active_fds);

                        // Удалить данные о клиенте из массива clients
                        // (может потребоваться дополнительная логика)
                        for (int i = 0; i < MAX_CLIENTS; ++i) {
                            if (clients[i].sockfd == fd) {
                                clients[i].sockfd = 0;
                                break;
                            }
                        }
                    } else {
                        printf("Data from client(fd = %d):\n", fd);

                        for (int i = 0; i < bytes_received; i++) {
                            printf("%c", toupper(buffer[i]));
                        }
                        printf("\n");
                    }
                }
            }
        }
    }

    // Закрытие всех активных сокетов
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].sockfd != 0) {
            close(clients[i].sockfd);
        }
    }

    // Закрытие сокета сервера
    close(sockfd);
    unlink("socket");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <ctype.h>
#include <sys/select.h>

#define SOCKET_PATH "/tmp/socket"

int main() 
{
    // Creating a soket
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error ofcreation a soket");
        exit(EXIT_FAILURE);
    }

    // Initializing a server's adress structure
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Binding a soket to the adress
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
     {
        perror("Error of binding a soket");
        exit(EXIT_FAILURE);
    }

    // Waiting for client's connections
    if (listen(sockfd, SOMAXCONN) == -1)
     {
        perror("Error of waiting for connection with clients");
        exit(EXIT_FAILURE);
    }

    // A set of file desctiptors for select
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    // Finding a max descriptor
    int maxfd = sockfd;

    while (1) 
    {
        fd_set tmpfds = readfds;

        // Applying select for waiting for any activities from descriptors
        if (select(maxfd + 1, &tmpfds, NULL, NULL, NULL) == -1) {
            perror("Error of applying select");
            exit(EXIT_FAILURE);
        }

        // Processing descriptor's activities
        for (int fd = 0; fd <= maxfd; ++fd)
         {
            if (FD_ISSET(fd, &tmpfds)) 
            {
                // For new connections
                if (fd == sockfd) 
                {
                    // Accepting a client
                    int clientfd = accept(sockfd, NULL, NULL);
                    if (clientfd == -1)
                     {
                        perror("Error of acception");
                        exit(EXIT_FAILURE);
                    }

                    // Adding client's descriptor to the set
                    FD_SET(clientfd, &readfds);

                    // Renew a max descriptor
                    if (clientfd > maxfd) maxfd = clientfd;
                }
                // For already added clients
                else 
                {
                    char buffer[256];
                    int bytes_read = read(fd, buffer, sizeof(buffer));
                    if (bytes_read == -1) {
                        perror("Error of reading the data from client");
                        exit(EXIT_FAILURE);
                    } 
                    else if (bytes_read == 0) 
                    {
                        // Closing connection with the client
                        close(fd);
                        FD_CLR(fd, &readfds);
                    } 
                    else 
                    {                    
                        // Processing the text
                        for (int i = 0; i < bytes_read; ++i)
                            buffer[i] = toupper(buffer[i]);
                        write(STDOUT_FILENO, buffer, bytes_read);
                    }
                }
            }
        }
    }

    return 0;
}
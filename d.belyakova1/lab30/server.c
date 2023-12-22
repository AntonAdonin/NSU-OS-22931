#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

char* socket_path = "./socket30";

int main() 
{
    char buf[256];
    int endpoint, socketDescriptor, dataCount;

    if ((endpoint = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket error");
        exit(-1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    unlink(socket_path);

    if (bind(endpoint, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
    {
        perror("Bind error");
        exit(-1);
    }

    if (listen(endpoint, 5) == -1) 
    {
        perror("Listen error");
        exit(-1);
    }

    while (1) 
    {
        if ((socketDescriptor = accept(endpoint, NULL, NULL)) == -1) 
        {
            perror("Accept error");
            continue;
        }

        while ((dataCount = read(socketDescriptor, buf, sizeof(buf))) > 0) 
        {
            for (int i = 0; i < dataCount; i++) 
            {
                buf[i] = (char)toupper(buf[i]);
                printf("%c", buf[i]);
            }
        }

        if (dataCount == 0) 
        {
            printf("EOF reached, closing connection\n");
            close(socketDescriptor);
        }

        else if (dataCount == -1) 
        {
            perror("don't read");
            exit(-1);
        }
    }
}
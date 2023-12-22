#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

char* socket_path = "./socket30";

int main() 
{
    char buf[256];
    int endpoint, dataCount;

    if ((endpoint = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
    {
        perror("Socket error");
        exit(-1);
    }

    struct sockaddr_un client;
    memset(&client, 0, sizeof(client));
    client.sun_family = AF_UNIX;
    strncpy(client.sun_path, socket_path, sizeof(client.sun_path) - 1);

    if (connect(endpoint, (struct sockaddr*)&client, sizeof(client)) == -1) 
    {
        perror("Connect error");
        exit(-1);
    }

    else 
    {
        while ((dataCount = read(STDIN_FILENO, buf, sizeof(buf))))
        {
            if (write(endpoint, buf, dataCount) != dataCount) 
            {
                if (dataCount > 0) fprintf(stderr, "Not full write error");

                else 
                {
                    perror("Write error");
                    exit(-1);
                }
            }
        }
    }

    return 0;
}
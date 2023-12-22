#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <aio.h>
#include <signal.h>

void io_handler(int signum, siginfo_t* info, void* context)
{
    // Getting a pointer to the aiocb structure (asynchronous I/O operation) from the si_value field of the info structure
    struct aiocb* current = info->si_value.sival_ptr;
    if (aio_error(current) == 0) 
    {
        size_t n_read = aio_return(current);
        char* buffer = (char*)current->aio_buf;
        if (n_read == 0 || *buffer == '\a') 
        {
            close(current->aio_fildes);
            free(buffer);
            free(current);
        }
        else 
        {
            size_t i;
            for (i = 0; i < n_read; ++i)
                printf("%c", toupper(buffer[i]));
            aio_read(current);
        }
    }
    else perror("Error of aio processing");
}

int main() 
{
    // Creating a soket
    int fds;
    if ((fds = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
    {
        perror("Error of creation a soket");
        return EXIT_FAILURE;
    }

    // Initializing a server's adress structure
    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, "socket", sizeof(address.sun_path) - 1);

    // Binding a soket to the adress
    unlink(address.sun_path);
    if (bind(fds, (struct sockaddr*)&address, sizeof(address)) == -1) 
    {
        perror("Error of binding a soket");
        close(fds);
        return EXIT_FAILURE;
    }

    // Waiting for client's connections
    if (listen(fds, 3) == -1) 
    {
        perror("Error of waiting for connection with clients");
        close(fds);
        return EXIT_FAILURE;
    }

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_sigaction = io_handler;
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGIO, &act, NULL);

    while (1) 
    {
        // Accepting a client
        int c_fds;
        if ((c_fds = accept(fds, NULL, NULL)) == -1) 
        {
            perror("Error of acception");
            close(fds);
            return EXIT_FAILURE;
        }

        // Processing
        struct aiocb* cb = calloc(1, sizeof(struct aiocb));
        cb->aio_fildes = c_fds;
        cb->aio_buf = malloc(255);
        cb->aio_nbytes = 255;
        cb->aio_sigevent.sigev_signo = SIGIO;
        cb->aio_sigevent.sigev_value.sival_ptr = cb;

        if (aio_read(cb) == -1) 
        {
            perror("Failed to queue the read request: ");
            close(fds);
            close(c_fds);
        }
    }
}
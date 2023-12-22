#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    long displ[500];
    int fd, fdcons, nom, line_ln[500];
    char c, buf[257];
    if ((fdcons = open("/dev/tty", O_RDONLY)) == -1)
    {
        perror("/dev/tty");
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        perror(strcat("Invailid file: ", argv[1]));
        exit(1);
    }
    displ[1] = 0L;
    int i = 1, j = 0;
    while (read(fd, &c, 1))
    {
        j++;
        if (c == '\n')
        {
            line_ln[i++] = j;
            displ[i] = lseek(fd, 0L, 1);
            j = 0;
        }
    }
    while (1)
    {
        printf("enter a line number in 5 sec\n");
        sleep(5);
        if ((i = read(fdcons, buf, 257)) == 0)
        {
            lseek(fdcons, SEEK_SET, 0);
            while ((i = read(fd, buf, 257)) > 0)
                write(1, buf, i);
            exit(0);
        }
        else
        {
             buf[i] = '\0';
             nom = atoi(buf);
             if (nom <= 0) exit(0);
             lseek(fd, displ[nom], 0);
             if (read(fd, buf, line_ln[nom])) write(1, buf, line_ln[nom]);
             else fprintf(stderr, "Incorrect line number\n");
        }
    return 0;
}

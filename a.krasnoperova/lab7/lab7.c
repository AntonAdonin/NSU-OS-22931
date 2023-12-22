#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

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
    int i = 1, j = 1;
    off_t size = lseek(fd, 0, SEEK_END);
    p = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    displ[1] = p;
    for (int count = 0; count < size; count++)
         if (*(p + count) == '\n')
         {
             line_ln[i++] = j;
             displ[i] = count + p + 1;
             j = 1;
         }
         else j++;
     displ[i] = 0;

    while (1)
    {
        printf("enter a line number in 5 sec\n");
        sleep(5);
        if ((i = read(fdcons, buf, 257)) == 0)
        {
            while ((i = read(fd, buf, 257)) > 0)
                write(1, buf, i);
            exit(0);
        }
        else
        {
            buf[i] = '\0';
            nom = atoi(buf);
            if (nom <= 0) exit(0);
            if (displ[nom]) write(1, displ[nom], line_ln[nom]);
            else fprintf(stderr, "Incorrect line number\n");
        }
        return 0;
    }

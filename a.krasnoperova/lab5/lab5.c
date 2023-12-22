#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
     long displ[500];
     int fd, nom, line_ln[500];
     char c, buf[257];
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
     while (printf("line number : ") && scanf("%d", &nom))
     {
         if (nom <= 0) exit(0);
         lseek(fd, displ[nom], 0);
         if (read(fd, buf, line_ln[nom])) write(1, buf, line_ln[nom]);
         else fprintf(stderr, "Incorrect line number\n");
     }
     return 0;
}

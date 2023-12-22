#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char filename[] = "file.txt";
    int fd, line_num, i;
    off_t *offsets;
    int *line_lengths;
    char line[256];
    fd = open(filename, O_RDONLY);
    off_t file_size = lseek(fd, 0, SEEK_END);
    int num_lines = 0;
    offsets = (off_t *)malloc(file_size * sizeof(off_t));
    line_lengths = (int *)malloc(file_size * sizeof(int));
    lseek(fd, 0, SEEK_SET);
    offsets[num_lines] = 0;
    while (read(fd, line, 1) > 0) {
        if (line[0] == '\n') {
            line_lengths[num_lines] = lseek(fd, 0, SEEK_CUR) - offsets[num_lines] - 1;
            num_lines++;
            offsets[num_lines] = lseek(fd, 0, SEEK_CUR);
        }
    }
    while (1) {
        printf("String number: ");
        scanf("%d", &line_num);
        if (line_num == 0) {
            break;
        }
        if (line_num < 0 || line_num > num_lines) {
            printf("Wrong number\n");
            continue;
        }
        lseek(fd, offsets[line_num - 1], SEEK_SET);
        read(fd, line, line_lengths[line_num - 1]);
        line[line_lengths[line_num - 1]] = '\0';
        printf("String  %d: %s\n", line_num, line);
    }
    close(fd);
    return 0;
}

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int time_out;

typedef struct {
    off_t offset;
    off_t length;
} Line;

typedef struct {
    Line *array;
    int length;
    int capacity;
    int size;
} Array;

void initArray(Array *a) {
    a->array = malloc(sizeof(Line));
    a->length = 0;
    a->capacity = 1;
    a->size = 0;
}

void insertArray(Array *a, Line element) {
    if (a->length == a->capacity) {
        a->capacity *= 2;
        a->array = realloc(a->array, a->capacity * sizeof(Line));
    }

    a->array[a->length++] = element;
    a->size += 1;
}

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->length = a->capacity = 0;
}

void printLine(Array* arr, int file, int i) {
    Line line = arr->array[i];
    char *buf = calloc(line.length, sizeof(char));

    lseek(file, line.offset, SEEK_SET);
    read(file, buf, (line.length) * sizeof(char));
    printf("%s\n\n", buf);

    free(buf);
}

void timer(int signum) {
    if (signum == SIGALRM) {
        time_out = 1;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) { return 1; }

    Array table;
    initArray(&table);

    int file = open(argv[1], O_RDONLY);
    if (file == -1) { exit(1); }
//    lseek(file, 0L, SEEK_CUR);

    char c;
    int lineOffset = 0;
    int lineLength = 0;
    while (read(file, &c, 1) == 1) {
        if (c == '\n') {
            Line current = {lineOffset, lineLength};
            insertArray(&table, current);

            lineOffset += lineLength + 1;
            lineLength = 0;
        } else {
            lineLength++;
        }
    }

    if (lineLength > 0) {
        Line current = {lineOffset, lineLength};
        insertArray(&table, current);
    }

    int num;
    signal(SIGALRM, timer);
    while (1) {
        printf("Enter the line number(0 to leave): ");
        alarm(5);
        time_out = 0;
        scanf("%d", &num);

        if(time_out){
            printf("...\n");
            for (int i = 0; i < table.size; i++) {
                printf("%d: ", i);
                printLine(&table, file, i);
            }
            break;
        }

        if (num == 0) { break; }
        if (table.length < num) {
            printf("The file contains only %d line(s).\n", table.length);
            continue;
        }

        printLine(&table, file, num - 1);
    }

    close(file);
    freeArray(&table);

    return 0;
}

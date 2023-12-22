#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct {
    off_t offset;
    off_t length;
} Line;

typedef struct {
    Line *array;
    int length;
    int capacity;
} Array;

void initArray(Array *a) {
    a->array = malloc(sizeof(Line));
    a->length = 0;
    a->capacity = 1;
}

void pushArray(Array *a, Line element) {
    if (a->length == a->capacity) {
        a->capacity *= 2;
        a->array = realloc(a->array, a->capacity * sizeof(Line));
    }

    a->array[a->length++] = element;
}

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->length = a->capacity = 0;
}

int main(int argc, char **argv) {
    if (argc != 2) { return 1; }

    Array table;
    initArray(&table);

    int file = open(argv[1], O_RDONLY);
    if (file == -1) { return 1; }
    lseek(file, 0L, SEEK_CUR);

    char c;
    int lineOffset = 0;
    int lineLength = 0;
    while (read(file, &c, 1) == 1) {
        if (c == '\n') {
            Line current = {lineOffset, lineLength};
            pushArray(&table, current);

            lineOffset += lineLength + 1;
            lineLength = 0;
        } else {
            lineLength++;
        }
    }

    if (lineLength > 0) {
        Line current = {lineOffset, lineLength};
        pushArray(&table, current);
    }

    while (1) {
        int num;
        printf("Enter the line number(0 to leave): ");
        scanf("%d", &num);

        if (num == 0) { break; }
        if (table.length < num) {
            printf("The file contains only %d lines.\n", table.length);
            continue;
        }

        Line line = table.array[num - 1];
        char *buf = calloc(line.length + 1, sizeof(char));

        lseek(file, line.offset, SEEK_SET);
        read(file, buf, (line.length + 1) * sizeof(char));

        printf("%s\n", buf);
        free(buf);
    }

    close(file);
    freeArray(&table);

    return 0;
}

//7. Таблица поиска строк в текстовом файле 2.
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct {  // содержит информацию об смещении и длине строки в файле
    off_t offset;  // смещение строки
    off_t length; // длина строки
} Line;

typedef struct {  // используется для хранения массива элементов типа `Line`
    Line* array;  // массив элементов Line
    int cnt;  // количество элементов в массиве
    int capacity; // емкость массива
} Array;

void freeArray(Array* a) {  // освобождает память, выделенную для массива Line в структуре Array
    free(a->array);
    a->array = NULL;
    a->cnt = 0;
    a->capacity = 0;
}

void insertArray(Array* a, Line element) {  // вставляет новый элемент Line в массив Array
    if (a->cnt == a->capacity) {  // если массив заполнен, его емкость удваивается
        a->capacity *= 2;
        a->array = realloc(a->array, a->capacity * sizeof(Line));
    }
    a->array[a->cnt++] = element;
}


void initArray(Array* a) {  // выделение памяти для массива Line
    a->array = malloc(sizeof(Line));
    a->cnt = 0;
    a->capacity = 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2)
        return 1;
    char* path = argv[1];

    int file = open(path, O_RDONLY);
    if (file == -1)
        return 1;

    struct stat file_info;
    if (fstat(file, &file_info) == -1) 
        return 1;
    size_t size = file_info.st_size;

    const char* map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, file, 0);
    if (map == MAP_FAILED) 
        return 1;

    Array table;  // table типа Array
    initArray(&table);  // выделение памяти для поля array

    off_t line_offset = 0, line_len = 0;
    for (int i = 0; i < size; i++) {
        char c = map[i];
        if (c == '\n') {  // если есть перевод строки,
            Line current = { line_offset, line_len };
            insertArray(&table, current);  // то строка записывается и

            line_offset += line_len + 1;
            line_len = 0;
        }
        else
            line_len++;
    }

    if (line_len > 0) {
        Line current = { line_offset, line_len };
        insertArray(&table, current);
    }

    fd_set fdset;
    struct timeval timer;

    while (1) {
        printf("Enter the number of line: ");

        fflush(stdout);

        FD_ZERO(&fdset);
        FD_SET(0, &fdset);
        timer.tv_sec = 5;
        timer.tv_usec = 0;

        if (!select(1, &fdset, NULL, NULL, &timer)) {
            printf("\n\tAll lines:\n");
            for (int i = 0; i < table.cnt; i++) {
                for (int j = 0; j < table.array[i].length; j++) {
                    printf("%c", map[table.array[i].offset + j]);
                }
                printf("\n");
            }
            return 0;
        }

        int number;
        scanf("%d", &number);

        if (number == 0)  // если номер строки равен нулю, то программа завершается
            break;
        if (number > table.cnt) {  // если введенный номер больше возможных строк, то запрос повторяется
            printf("Total lines = %d\n", table.cnt);
            continue;
        }

        Line line = table.array[number - 1];  // создается еще одна строка, в которую записывается введенная строка
        for (int i = 0; i < line.length; i++)
            printf("%c", map[line.offset + i]);
        printf("\n");
    }

    munmap((void*)map, size);
    freeArray(&table);

    return 0;
}
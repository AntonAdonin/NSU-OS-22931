#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

typedef struct RowParams_s{
    off_t offset;
    size_t lenght;
} RowParams;

typedef struct Vector_s{
    RowParams* vec;
    size_t lenght, max_lenght;
} Vector;

int fd;
Vector vec;

Vector* init_vector(){
    Vector* vec = (Vector*)calloc(1, sizeof(Vector));
    vec->vec = (RowParams*)malloc(sizeof(RowParams));
    vec->max_lenght = 1;
    return vec; 
}

void push_back(Vector* params, RowParams val){
    if (params->max_lenght == params->lenght){
        params->max_lenght *= 2;
        params->vec = (RowParams*)realloc(params->vec, sizeof(RowParams) * params->max_lenght);
    }
    params->vec[params->lenght++] = val;
}

Vector* get_row_params(){
    off_t offset = 0;
    size_t lenght = 0;
    Vector* params = init_vector();
    while(1){
        char c;
        if (pread(fd, &c, 1, offset) <= 0) {
            RowParams val = {offset - lenght, lenght};
            push_back(params, val);
            break;
        }
        if (c == '\n') {
            RowParams val = {offset - lenght, lenght};
            push_back(params, val);
            lenght = -1;
        }
        offset++, lenght++;
    }
    return params;
}

void print_vector(Vector* vec){
    for (int i = 0; i < (int)vec->lenght; i++){
        RowParams temp = vec->vec[i];
        printf("%ld %ld\n", temp.offset, temp.lenght);
    }
}

void print_line(RowParams param){
    char* str = (char*)calloc(param.lenght + 1, sizeof(char));
    lseek(fd, param.offset, SEEK_SET);
    read(fd, str, param.lenght);
    printf("%s\n", str);
}

void print_all_lines(int sig){
    for (size_t i = 0; i < vec.lenght; i++) print_line(vec.vec[i]);
    exit(0);
}

void get_lines(){
    Vector* a = get_row_params();
    vec = *a;
    signal(SIGALRM, print_all_lines);
    //print_vector(a);
    while(1){
        printf("Input number of row: ");
        char num[8];
        alarm(5);
        fgets(num, 8, stdin);
        int ind = atoi(num);
        if (ind == 0) break;
        if ((size_t)ind > a->lenght) printf("This line doesn't exist\n");
        else print_line(a->vec[--ind]);
    }

    free(a);
}

int main() {
    fd = open("input.txt", O_RDONLY);

    if (fd == -1) {
        perror("open not found");
        exit(404);
    }

    get_lines(fd);

    close(fd);

    return 0;
}
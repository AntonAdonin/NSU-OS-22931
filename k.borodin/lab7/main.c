#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define LINE_MAX 100


typedef struct Vector_s{
    char** vec;
    size_t lenght, max_lenght;
} Vector;

Vector vec;

void init_vector(){
    vec.vec = (char**)malloc(sizeof(char));
    vec.max_lenght = 1;
    vec.lenght = 0;
}

void push_back(char line[]){
    if (vec.max_lenght == vec.lenght){
        vec.max_lenght *= 2;
        vec.vec = (char**)realloc(vec.vec, sizeof(char*) * vec.max_lenght);
    }
    vec.vec[vec.lenght] = (char*)malloc(strlen(line) + 1);
    strcpy(vec.vec[vec.lenght++], line);
}

void get_rows(char file_name[]){
    FILE* input = fopen(file_name, "r");
    char line[LINE_MAX];
    while (fgets(line, sizeof(line), input)) {
        push_back(line);
    }

    fclose(input);
}

void print_vector(){
    for (int i = 0; i < (int)vec.lenght; i++){
        printf("%s", vec.vec[i]);
    }
    exit(0);
}

void get_lines(){
    init_vector();
    get_rows("input.txt");
    signal(SIGALRM, print_vector);
    while(1){
        printf("Input number of row: ");
        int num;
        alarm(5);
        scanf("%d", &num);
        int ind = num;
        if (ind == 0) break;
        if ((size_t)ind > vec.lenght) printf("This line doesn't exist\n");
        else printf("%s", vec.vec[--ind]);
    }
}

int main() {

    get_lines();

    return 0;
}
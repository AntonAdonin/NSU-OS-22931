#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_SIZE 100

typedef struct Node_s {
    char *string;
    struct Node_s *next;
} Node;

Node *head, *tail;
char *ptrCopy;

void push(char* str) {
    ptrCopy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(ptrCopy, str);

    tail->string = ptrCopy;
    tail->next = calloc(1, sizeof(Node));
    tail = tail->next;
}

void printList() {
    Node *ptr = head;
    printf("\nInputted Strings:\n");
    int i = 1;

    while (ptr != NULL) {
        if (ptr->string) {
            printf("%d:  %s\n", i++, ptr->string);
        }
        ptr = ptr->next;
    }
}

int main() {
    char input[MAX_STR_SIZE] = {0};

    head = calloc(1, sizeof(Node));
    tail = head;

    while (fgets(input, MAX_STR_SIZE, stdin) != NULL) {
        if (input[0] == '.') {
            printList();
            return 0;
        }

        char* end = strchr(input, '\n');
        *end = '\0';
        push(input);
    }
}
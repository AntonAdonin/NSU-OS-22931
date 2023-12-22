#define _CRT_SECURE_NO_WARNINGS
#define MAX_LIMIT 100000
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    struct Node* next;
    char* value;
} Node;

Node* create_node(int len, char* str) {
    Node* new_node = malloc(sizeof(Node));
    new_node->next = NULL;
    new_node->value = (char*)malloc(len + 1);
    strcpy(new_node->value, str);
    return new_node;
}

int main(){
    char* mas = (char*)malloc(sizeof(char) * MAX_LIMIT);
    Node* head = NULL;
    Node* curr = NULL;
    do {
        fgets(mas, MAX_LIMIT, stdin);
        int len = strlen(mas);
        if (head == NULL) head = create_node(len, mas);
        else if (head->next == NULL) {
            head->next = create_node(len, mas);
            curr = head->next;
        }
        else {
            curr->next = create_node(len, mas);
            curr = curr->next;
        }
    } while (mas[0] != '.');

    free(mas);

    curr = head;
    while (curr->next != NULL) {
        printf("%s", curr->value);
        curr = curr->next;
    }
    return 0;
}
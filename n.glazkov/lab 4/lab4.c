#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char* data;
    struct Node* next;
};

void insert(struct Node** head, char* data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = (char*)malloc(strlen(data) + 1);
    strcpy(newNode->data, data);
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

int main() {
    struct Node* head = NULL;
    char input[2048];
    printf("Insert strings:\n");
    while (1) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  
        if (strcmp(input, ".") == 0) {
            break;
        }
        insert(&head, input);
    }
    printf("Strings:\n");
    printList(head);
    return 0;
}

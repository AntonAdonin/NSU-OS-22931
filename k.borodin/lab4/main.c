#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Node_s{
	struct Node_s* next;
	char* val;
} Node;

typedef struct List_s{
	Node* head, *tail;
} List;

Node* create_node(char val[]){
       Node* curr = (Node*)calloc(1, sizeof(Node));
       curr->val = (char*)malloc(strlen(val) + 1);
       strcpy(curr->val, val);
       return curr;       
}

void add_to_list(List* list, char val[]){
	Node* curr = create_node(val);
	if (list->head == NULL) {
        list->head = curr;
        list->tail = list->head;	
    }
    else list->tail->next = curr, list->tail = curr;
}

void print_and_destroy(List* list){
	Node* curr = list->head;
	while (curr != NULL){
		printf("%s ", curr->val);
		free(curr->val);
		Node* prev = curr;
		curr = curr->next;
		free(prev);
	}
}


int main(){
    List* list = (List*)calloc(1, sizeof(List));
    char string[LINE_MAX];
    while(1) {
        fgets(string, LINE_MAX, stdin);
        if (strcmp(string, ".\n") == 0) break; 
        add_to_list(list, string);
    }
    print_and_destroy(list);
    return 0;
}

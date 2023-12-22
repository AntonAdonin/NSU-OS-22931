#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
	char* data;
	struct Node* next;
};

void insertNode(struct Node** head, char* data){
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = (char*)malloc(strlen(data)+1);
	strcpy(newNode->data, data);
	
	newNode->next = *head;
	*head = newNode;
}

void printList(struct Node* head){
	struct Node* current = head;

	while(current != NULL){
		printf("%s\n", current->data);
		current = current->next;
	}
}

int main(){
	struct Node* head = NULL;
	char input[100];
	printf("Input what you want(enter dot to stop):\n");
	while(1){
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0';
		if(input[0] == '.'){
			break;
		}
		insertNode(&head, input);
	}
	printf("Typed strings:\n");
	printList(head);
	return 0;
}

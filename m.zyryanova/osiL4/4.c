#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Line {
	char* str;
	int len;
}Line;

typedef struct List {
	Line* list;
	int len;
	int currentlen;
}List;



void initList(List* list) {
	list->len = 4;
	list->list = malloc(sizeof(Line) * list->len);
	list->currentlen = 0;
}

void printPointers(char** pointers, int len) {
	for (int i = 0; i < len; i++) {
		printf("%s", pointers[i]);
	}
}

void checkMemory(List list, char** pointers) {
	if (list.currentlen == list.len) {
		list.len *= 2;
		list.list = (Line*)calloc(list.len, sizeof(Line));
		pointers = (char**)calloc(list.len, sizeof(char*));
	}
}

int main(int argc, char* argv[]) {

	List list = {0,0,0};
	initList(&list);


	char** pointers = (char**)malloc(sizeof(char*) * list.currentlen);
	char* str = (char*)malloc(sizeof(char) * 256);

	fgets(str, 256, stdin);
	while (strcmp(str, ".\n") != 0) {
		checkMemory(list, pointers);
		list.list[list.currentlen].str = (char*)malloc(sizeof(char)* strlen(str));
		list.list[list.currentlen].len = strlen(str);
		strcpy(list.list[list.currentlen].str, str);
		pointers[list.currentlen] = &list.list[list.currentlen].str[0];
		list.currentlen++;
		fgets(str, 256, stdin);
	}

	printPointers(pointers, list.currentlen);
	return 0;
}
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node {
    char* strok;
    struct node* sled;
}el;

int main()
{
    char string[512];
    el* first = malloc(sizeof(el));
    first->sled = NULL;
    el* now = first;
    while (gets(string) != NULL)
    {
        if (string[0] == '.') break;
        now->sled = newel(string);
        now = now->sled;
    }
    for (el* p = first->sled; p != NULL; p = p->sled)
        puts(p->strok);
    return 0;
}



el* newel(char* str)
{
    el* a = malloc(sizeof(el));
    a->strok = malloc(strlen(str) + 1);
    strcpy(a->strok, str);
    a->sled = NULL;
    return(a);
}

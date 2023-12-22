#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    struct Node *next;
    char *str;
} Node;

typedef struct List
{
    Node *tail;
    Node *head;
    int size;
} List;

List read_lines(List L);
void print_list(List L);


int main()
{
    List L;
    L.size = 0;

    L = read_lines(L);
    print_list(L);

    return 0;
}


List read_lines(List L)
{
    char str[128];
    while (1)
    {
        fgets(str, 127, stdin);
        if (str[0] == '.')
            break;
        size_t len = strlen(str);

        char *s = (char *)malloc(sizeof(char) * (len + 1));

        if (L.size == 0)
        {
            Node *node = (Node *)malloc(sizeof(Node));
            node->str = s;
            node->next = NULL;
            L.head = node;
            L.tail = node;
            L.size = 1;
            strcpy(s, str);
            continue;
        }
        Node *node = (Node *)malloc(sizeof(Node));
        node->str = s;
        strcpy(s, str);
        node->next = NULL;
        L.tail->next = node;
        L.tail = L.tail->next;
    }
    return L;
}

void print_list(List L)
{
    printf(" ------------------ \n");
    Node *r = L.head;
    while (r != NULL)
    {
        printf("%s\n", r->str);
        r = r->next;
    }
}

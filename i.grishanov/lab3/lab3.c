#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void open_file(FILE* f);
void init_file();
void print_id();

int main() {

    init_file();
    print_id();
    FILE* f;
    open_file(f);
    setuid(getuid());
    print_id();
    open_file(f);
    return 0;
}

void print_id()
{
    printf("real usr ID = %d\n", getuid());
    printf("effective usr ID = %d\n", geteuid());
}

void init_file()
{
    system(">file.txt");
    system("echo \"this is a test text\" > file.txt");
    system("chmod 600 file.txt");
}

void open_file(FILE* f)
{

    if((f = fopen("file.txt", "r")) == NULL)
        perror("Cannot open file");
    fclose(f);
}
#include <unistd.h>
#include <stdio.h>

void printUID() {
    printf("uid: %d, euid: %d\n", getuid(), geteuid());
}

void openFile(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open the file\n");
        return;
    }

    fclose(file);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\nFilename is missing\n");
        return 1;
    }

    printUID();
    openFile(argv[1]);

    setuid(getuid());

    printUID();
    openFile(argv[1]);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

int main()
{
    uid_t real_uid, effective_uid;
    FILE *file;

    real_uid = getuid();
    effective_uid = geteuid();
    printf("Real UID: %d\n", real_uid);
    printf("Effective UID: %d\n", effective_uid);

    file = fopen("file", "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    setuid(real_uid);

    real_uid = getuid();
    effective_uid = geteuid();
    printf("Real UID: %d\n", real_uid);
    printf("Effective UID: %d\n", effective_uid);

    fclose(file);

    return 0;
}

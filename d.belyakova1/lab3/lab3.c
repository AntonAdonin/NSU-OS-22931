#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

int main()
{
    struct stat info;
    stat("file.txt", &info);
    struct passwd *ownerData = getpwuid(info.st_uid);
    unsigned int ownerUid = ownerData->pw_uid;

    printf("%d %d\n",geteuid(), getuid());

    if (getuid() == ownerUid)
    {
        FILE* nice = fopen("file.txt", "r");
        fclose(nice);
    }

    else
    {
        perror("You are not the owner");
    }

    setuid(geteuid());
    printf("%d %d\n",geteuid(), getuid());

    return 0;
}
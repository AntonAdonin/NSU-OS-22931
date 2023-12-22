#include <stdlib.h>
#include <ulimit.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/resource.h>

extern char** environ;

int main(int argc, char* argv[])
{
    char options[] = "ispuU:cC:dvV:";  /* valid options */
    int c, a;
    char* f_ptr, * g_ptr;
    struct rlimit rlp;
    char** p;
    printf("argc equals %d\n", argc);

    if (argc < 2)
    {
        fprintf(stderr, "%s\n", argv[0]);
        return 0;
    }

    while ((c = getopt(argc, argv, options)) != EOF)
    {
        switch (c)
        {
        case 'i':
            printf("The real user d is %ld\nThe effective user id is %ld\nThe real group id is %ld\nThe effective group id is %ld\n", getuid(), geteuid(), getgid(), getegid());
            break;
        case 's':
            setpgid(0, 0);
            printf("The process has become the leader of the group\n");
            break;
        case 'p':
            printf("The process's id is %ld\nThe parent-process's id is %ld\nThe id of group of the processes is %ld\n", getpid(), getppid(), getpgid(0));
            break;
        case 'u':
            printf("The ulimit value is %ld\n", ulimit(1, 0));
            break;
        case 'U':
            a = ulimit(2, atol(optarg));
            if (a == -1) fprintf(stderr, "Not a super-user\n");
            break;
        case 'c':
            getrlimit(RLIMIT_CORE, &rlp);
            printf("The core size is %ld\n", rlp.rlim_cur);
            break;
        case 'C':
            getrlimit(RLIMIT_CORE, &rlp);
            rlp.rlim_cur = atol(optarg);
            a = setrlimit(RLIMIT_CORE, &rlp);
            if (a == -1) fprintf(stderr, "Not a super-user\n");
            break;
        case 'd':
            printf("The current working directory is: %s\n", getcwd(NULL, 100));
            break;
        case 'v':
            printf("Environment variables are: \n");
            for (p = environ; *p != 0; p++)
                printf("%s\n", *p);
            break;
        case 'V':
            putenv(optarg);
            printf("The variable is added\n");
            break;
        case '?':
            printf("invalid option is %c\n", optopt);
        }
    }
    return 0;
}

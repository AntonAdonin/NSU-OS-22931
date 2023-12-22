#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>

struct rlimit core_lim;

int main(int argc, char *argv[]) {
    char options[] = "ispuU:cC:dvV";  /* valid options */
    int c;

    extern char **environ;
    extern char *optarg;

    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            // Print the Real and Effective User and Group ID
            case 'i': {
                uid_t ruid = getuid(), euid = geteuid();
                gid_t rgid = getgid(), egid = getegid();

                printf("\nReal UID: %d\nEffective UID: %d\nReal GID: %d\nEffective GID: %d\n", ruid, euid, rgid, egid);

                break;
            }

                // Set the group leader process
            case 's': {
                if (-1 == setpgid(0, 0)) {
                    printf("Failed to set the group leader process\n");
                } else {
                    printf("The group leader process has been set successfully\n");
                }

                break;
            }

                // Print the Process ID, Parent Process ID and Process Group ID
            case 'p': {
                pid_t pid = getpid(), gpid = getpgrp(), ppid = getppid();
                printf("\nProcess ID: %d\nParent Process ID: %d\nProcess Group ID: %d\n", pid, ppid, gpid);

                break;
            }
                // Print the ulimit value
            case 'u': {
                /*struct rlimit rlim;
                if (-1 == getrlimit(RLIMIT_FSIZE, &rlim)) {
                    printf("\nFailed to get ulimit\n");
                } else {
                    printf("\nulimit: %ld\n", rlim.rlim_max);
                }*/

                long ulim = ulimit(UL_GETFSIZE);
                printf("\nulimit value is: %ld\n", ulim);

                break;
            }

                // Set new ulimit value
            case 'U': {
                if (ulimit(UL_SETFSIZE, atol(optarg)) == -1) {
                    printf("\nFailed to set new ulimit\n");
                }

                else {
                    printf("\nUlimit changed to %ld\n", ulimit(UL_GETFSIZE));
                }

                break;
            }

                // Print core-file size limit
            case 'c': {
                if (getrlimit(RLIMIT_CORE, &core_lim) == -1) {
                    printf("\nFailed to get the core-file size limit\n");
                } else {
                    printf("\nCore-file size limit: %ld\n", core_lim.rlim_max);
                }

                break;
            }

                // Set new core-file size limit
            case 'C': {
                core_lim.rlim_cur = atol(optarg);
                if (setrlimit(RLIMIT_CORE, &core_lim) == -1) {
                    printf("\nFailed to set core limit\n");
                } else {
                    printf("\nNew core-file size limit has set\n");
                }

                break;
            }

                // Print the current directory
            case 'd': {
                char *pathname = getenv("PWD");

                if (NULL == pathname) {
                    printf("\nFailed to get the current directory\n");
                } else {
                    printf("\nCurrent directory: %s\n", pathname);
                }

                break;
            }

                // Print environmental variables and their values
            case 'v': {
                printf("\nCurrent envs:\n");
                int i = 0;
                while(environ[i] != 0) {
                    printf("%d      %s\n", i, environ[i]);
                    i++;
                }

                break;
            }

                // Set a new environmental variable or changes the value of an existing variable
            case 'V': {
                putenv(optarg);
                printf("\nNew env added successfully\n");

                break;
            }

            case ':': {
                printf("\nMissing argument for option: %c\n", optopt);

                break;
            }
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ulimit.h>
#include <limits.h>
#include <sys/resource.h>

#define NUMGROUPS 16
#define _MAX_PATH 260 /* max. length of full pathname */
int main(int argc, char *argv[], char *envp[])
{
    char options[] = "ispucdvU:C:V:"; /* vallid operations*/
    int c = 0;
    while ((c = getopt(argc, argv, options)) != -1)
    {
        switch (c)
        {

            // get real and effiective user & group id
        case 'i': 
        {
            printf("real usr ID = %d\n", getuid());
            printf("effective usr ID = %d\n", geteuid());
            printf("group id: %d\n", getgid());
            printf("effective group id: %d\n", getegid());
            break;
        }
        
            // the process becomes the group leader 
        case 's':
        {
            if(setpgid(0, 0) == -1) {perror("failed to set the group leader process\n");}
            
            else {printf("success\n");}
            break;
        }

            // get process, process-parent and process-group ID
        case 'p':
        {
            printf("process id: %d\n", getpid());
            printf("process parent id: %d\n", getppid());
            printf("process group id: %d\n", getpgrp());
            break;
        }

            // print ulimit value
        case 'u':
        {
            printf("ulimit is: %ld\n", ulimit(UL_GETFSIZE));
        }

            // print current directory 
        case 'd':
        {
            char PATH[_MAX_PATH];
            if(getcwd(PATH, _MAX_PATH) == NULL) {perror("failed to get the cur dir\n");}
            
            else {printf("your current dir: %s\n", getcwd(PATH, _MAX_PATH));} 
            break;
        }

            // print all environmental values
        case 'v':
        {
            printf("environment:\n");
            int i = 0;
            
            while (envp[i] != NULL)
            {
                printf("%s\n", envp[i]);
                i++;
            }
            break;
        }

            // print the core-file size which could be created (in bytes)
        case 'c':
        {
            struct rlimit rlp;
            if(getrlimit(RLIMIT_CORE, &rlp) == -1) {perror("failed to get rlimitCore\n");}
            
            else {printf("max core-file size: %lu\n", rlp.rlim_max);}
            break;
        }

            // change core-file size
        case 'C':
        {
            struct rlimit rlp;
            long long new_rlim = atoll(optarg);
            if (new_rlim = 0) {perror("size != 0");}

            else 
            {
                rlp.rlim_cur = new_rlim;
                setrlimit(RLIMIT_CORE, &rlp);
                printf("new rlimit was setted\n");
            }   
            break;
        }

            // change ulimit value
        case 'U':
        {
            unsigned long new_ulimit;
            new_ulimit = atol(optarg);
            if(new_ulimit <= 0) {perror("invalid arg for -U option \n(failed to set new max file size)\n");}
            
            else 
            {
                ulimit(UL_SETFSIZE, new_ulimit);
                printf("New maximum file size: %ld\n", ulimit(UL_GETFSIZE));
            }
            break;
        }

            // create new env value or change the existing one
        case 'V':
        {
            if (-1 == putenv(optarg))
            {
                printf("failed to set the environmental variable");
            }
            break;
        }

        case ':':
        {
            printf("missing argument for option: %c\n", optopt);
            break;
        }
        
        default:
        {
            printf("invalid option: %c\n", optopt);
        }
            
        }
    }
        return 0;
    }
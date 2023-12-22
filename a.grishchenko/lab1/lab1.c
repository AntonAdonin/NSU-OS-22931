#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <ulimit.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    int option;
    while ((option = getopt(argc, argv, "ispuU:cC:dvV:")) != -1) {
	uid_t uid = getuid();
        uid_t euid = geteuid();
        gid_t gid = getgid();
        gid_t egid = getegid();
        pid_t pid = getpid();
        pid_t ppid = getppid();
        pid_t pgid = getpgid(pid);
	struct rlimit rlm;


        switch (option) {
            case 'i':;
                uid = getuid();
        	euid = geteuid();
        	gid = getgid();
                egid = getegid();
                printf("Real User ID: %d\n", uid);           
        	printf("Effective User ID: %d\n", euid);    
        	printf("Real Group ID: %d\n", gid); 
            	printf("Effective Group ID: %d\n", egid);
                printf("Option -i selected\n");
                break;
            case 's':;
	        pid = getpid();
	        printf("%d %s", pid, "- leader of the group \n");
                printf("Option -s selected\n");
                break;
            case 'p':;
                ppid = getppid();
                pgid = getpgid(pid);
                printf("PID: %d\n", pid);
                printf("PPID: %d\n", ppid);
                printf("PGID: %d\n", pgid);
                printf("Option -p selected\n");
                break;
            case 'u':;
        	printf("Ulmit = %ld\n", ulimit(UL_GETFSIZE));
                printf("Option -u selected\n");
                break;
            case 'U':;
		if (ulimit(UL_SETFSIZE, atol(optarg)) == -1){
               		printf("error\n");
               		return 1;
          	}
          	printf("New ulmit = %ld\n\n", ulimit(UL_SETFSIZE));
          	break;
            case 'c':;
	        getrlimit(RLIMIT_FSIZE, &rlm);
	        printf("Core file size = %ld\n", rlm.rlim_cur);
                printf("Option -c selected\n");
                break;
            case 'C':;
        	getrlimit(RLIMIT_FSIZE, &rlm);
          	rlm.rlim_cur = atol(optarg);
          	if(setrlimit(RLIMIT_FSIZE, &rlm)==-1){
			printf("error\n");
			return 1;
		}		
		else{
	        	printf("new core file size = %ld\n", rlm.rlim_cur); 
        	        printf("Option -C selected \n");
                	break;
		}
            case 'd':;
        	char cwd[1024];
        	printf("Current work directory: %s\n", getcwd(cwd, sizeof(cwd)));
                printf("Option -d selected\n");
                break;
            case 'v':;
        	extern char **environ;
		int i = 0;
		while(environ[i] != NULL){
			printf("%s\n", environ[i]);
			i++;
		}
                printf("Option -v selected\n");
                break;
            case 'V':
           	putenv(optarg);
           	printf("Added new env %s\n\n", optarg);
                printf("Option -V selected: %s\n", optarg);
                break;
            default:
                printf("Invalid option\n");
                return 1;
        }
    }
    return 0;
}
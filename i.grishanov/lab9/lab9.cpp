#include <iostream>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) 
{
    if(argc < 2 || argc >= 3){
        perror("invalid arguments");
        return 1;
    }
    
    std::string tmp = "cat " + static_cast<std::string>(argv[1]);
    
    char* cmd = (char*)malloc(sizeof(tmp));
    strcpy(cmd, tmp.c_str());

    pid_t p = fork(), w;
    int status;

    if(p < 0){
        perror("fork failed");
        return 1;
    }

    if(p == 0){
        std::cout << "Child PID is " << getpid() << std::endl;
        system(cmd);
        exit(0);
    }

    else 
    {
        do
        {
            w = waitpid(p, &status, WUNTRACED | WCONTINUED);
            if(w == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            if(WIFEXITED(status))
                std::cout << std::endl << "Parent PID is " << getpid() << std::endl;
                
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        exit(EXIT_SUCCESS);
    }
}
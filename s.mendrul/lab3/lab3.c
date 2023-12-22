#include <unistd.h>
#include <stdio.h>

void prUID(){
    printf("uid: %d, euid:: %d\n", getuid(), geteuid());
}

void open_file(char* filename){
    FILE* file = fopen(filename, "r");
    
    if(file == NULL){
        perror("Filed to open file\n");
        return;
    }
    fclose(file);
}

int main(int argc, char** argv){
    if(argc < 2){
        printf("Filename is required\n");
        return 1;    
    }
    
    prUID();
    open_file(argv[1]);
    setuid(getuid());
    
    prUID();
    open_file(argv[1]);

    return 0;
}

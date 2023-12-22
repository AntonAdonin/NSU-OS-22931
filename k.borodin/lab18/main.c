#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include <libgen.h>

static struct stat st;



void get_access_rights(char* access_rights) {
    access_rights[0] = st.st_mode & S_IRUSR ? 'r' : '-';
    access_rights[1] = st.st_mode & S_IWUSR ? 'w' : '-';
    access_rights[2] = st.st_mode & S_IXUSR ? 'x' : '-';
    access_rights[3] = st.st_mode & S_IRGRP ? 'r' : '-';
    access_rights[4] = st.st_mode & S_IWGRP ? 'w' : '-';
    access_rights[5] = st.st_mode & S_IXGRP ? 'x' : '-';
    access_rights[6] = st.st_mode & S_IROTH ? 'r' : '-';
    access_rights[7] = st.st_mode & S_IWOTH ? 'w' : '-';
    access_rights[8] = st.st_mode & S_IXOTH ? 'x' : '-';
    access_rights[9] = '\0';
}

void get_owner_name(char* owner_name) {
    struct passwd* p = getpwuid(getuid());
    strncpy(owner_name, p->pw_name, 20);
}

void get_group_name(char* group_name) {
    struct group* g = getgrgid(getgid());
    strncpy(group_name, g->gr_name, 20);
}

void get_file_size(char file_status, char* file_size) {
    if (file_status == '-')
        snprintf(file_size, 10, "%ld", st.st_size);
    else
        *file_size = 0;
}

void get_file_status(char* file_status) {
    if (S_ISDIR(st.st_mode))
        *file_status = 'd';
    else if (S_ISREG(st.st_mode))
        *file_status = '-';
    else
        *file_status = '?';
}

void get_date(char* date) {
    struct tm* t = localtime(&st.st_mtime);
    strftime(date, 13, "%e %b %R", t);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("You need write the name of file");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = 1; i < argc; ++i) {
        stat(argv[i], &st);

        char access_rights[10];
        char file_status;
        char file_size[10];
        char date[13];
        char owner_name[33];
        char group_name[33];
        char* file_name;

        get_file_status(&file_status);
        get_access_rights(access_rights);
        get_owner_name(owner_name);
        get_group_name(group_name);
        get_file_size(file_status, file_size);
        get_date(date);
        file_name = basename(argv[i]);

        printf("%c%s %s %s %s %s %-s\n", file_status, access_rights, owner_name, group_name, file_size, date, file_name);
    }
    return EXIT_SUCCESS;
}
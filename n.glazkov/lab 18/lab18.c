#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

void print_mode(mode_t mode) {
    char permissions[10] = {0};
    int user_permissions = mode & S_IRUSR;
    int group_permissions = mode & S_IRGRP;
    int other_permissions = mode & S_IROTH;
    permissions[0] = (user_permissions | group_permissions | other_permissions) ? '-' : ' ';
    permissions[1] = (mode & S_IWUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[3] = (mode & S_IWOTH) ? 'x' : '-';
    permissions[4] = (mode & S_ISUID) ? 's' : ' ';
    permissions[5] = (mode & S_ISGID) ? 's' : ' ';
    permissions[6] = (mode & S_ISVTX) ? 't' : ' ';
    printf("%c", permissions[0]);
    printf("%c%c%c", (user_permissions & S_IRUSR) ? 'r' : '-', (user_permissions & S_IWUSR) ? 'w' : '-', (user_permissions & S_IXUSR) ? 'x' : '-');
    printf("%c%c%c", (group_permissions & S_IRGRP) ? 'r' : '-', (group_permissions & S_IWGRP) ? 'w' : '-', (group_permissions & S_IXGRP) ? 'x' : '-');
    printf("%c%c%c", (other_permissions & S_IROTH) ? 'r' : '-', (other_permissions & S_IWOTH) ? 'w' : '-', (other_permissions & S_IXOTH) ? 'x' : '-');
}

void print_owner_group(struct stat stat_buf) {
    uid_t owner = stat_buf.st_uid;
    gid_t group = stat_buf.st_gid;
    struct passwd *owner_pw = getpwuid(owner);
    struct group *group_gr = getgrgid(group);
    printf("%d (%s) ", owner, owner_pw->pw_name);
    printf("%d (%s) ", group, group_gr->gr_name);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }
    struct stat stat_buf;
    stat(argv[1], &stat_buf);
    print_mode(stat_buf.st_mode);
    printf(" %ld\n", stat_buf.st_nlink);
    print_owner_group(stat_buf);
    return 0;
}

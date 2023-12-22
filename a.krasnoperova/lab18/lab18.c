#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++)
    {
        struct stat file_stat;
		if (stat(argv[i], &file_stat) == -1)
        {
            perror("stat");
            continue;
        }
		else if(S_ISDIR(file_stat.st_mode)) printf("d");
		else if (S_ISREG(file_stat.st_mode)) printf("-");
		else printf("?");
		printf("%c%c%c", file_stat.st_mode & S_IRUSR ? 'r' : '-', .st_mode & S_IWUSR ? 'w' : '-', file_stat.st_mode & S_IXUSR ? 'x' : '-');
		printf("%c%c%c", file_stat.st_mode & S_IRGRP ? 'r' : '-', file_stat.st_mode & S_IWGRP ? 'w' : '-', file_stat.st_mode & S_IXGRP ? 'x' : '-');
		printf("%c%c%c", file_stat.st_mode & S_IROTH ? 'r' : '-', file_stat.st_mode & S_IWOTH ? 'w' : '-', file_stat.st_mode & S_IXOTH ? 'x' : '-');
		printf("%4ld", file_stat.st_nlink);
		struct passwd *owner = getpwuid(file_stat.st_uid);
		struct group *group = getgrgid(file_stat.st_gid);
		printf("%10s %10s", owner->pw_name, group->gr_name);
		if (S_ISREG(file_stat.st_mode)) printf("%10ld", file_stat.st_size);
		else printf("%10s", "");
		char date[20];
		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
		printf(" %s %s\n", date, argv[i]);
	}
	return 0;
}
#include <stdio.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <stdbool.h>

void IsDirectory(struct stat* fstat) {
  bool is_regular_file = (*fstat).st_mode & S_IFREG;
  if (is_regular_file) {
    printf("-");
    return;
  }

  bool is_directory = (*fstat).st_mode & S_IFDIR;
  if (is_directory)
    printf("d");
  else
    printf("?");
}

void PrintAccessStatus(struct stat* fstat, int mask, char flag) {
  bool is_this_access_allowed = (*fstat).st_mode & mask;
  if (is_this_access_allowed)
    printf("%c", flag);
  else
    printf("-");
}

void PrintLinkCount(struct stat* fstat) {
  printf(" %lu ", (long unsigned int) (*fstat).st_nlink);
}

void PrintOwnerAndGroup(struct stat* fstat) {
  struct group* user_group_info = getgrgid((*fstat).st_gid);
  struct passwd* user_info = getpwuid((*fstat).st_uid);
  printf("%s %s ", user_info->pw_name, user_group_info->gr_name);
}

void PrintSize(struct stat* fstat) {
  printf("%ld ", (*fstat).st_size);
}

void PrintLastModified(struct stat* fstat) {
  int const kDateTimeDescriptionSize = 50;

  struct tm t;
  localtime_r(&(*fstat).st_mtime, &t);
  char date[kDateTimeDescriptionSize];
  strftime(date, sizeof(date), "%b %d %H:%M", &t);
  printf("%s ", date);
}

void PrintFileStat(char* path) {
  struct stat fstat;
  stat(path, &fstat);

  IsDirectory(&fstat);

  PrintAccessStatus(&fstat, S_IRUSR, 'r');
  PrintAccessStatus(&fstat, S_IWUSR, 'w');
  PrintAccessStatus(&fstat, S_IXUSR, 'x');

  PrintAccessStatus(&fstat, S_IRGRP, 'r');
  PrintAccessStatus(&fstat, S_IWGRP, 'w');
  PrintAccessStatus(&fstat, S_IXGRP, 'x');

  PrintAccessStatus(&fstat, S_IROTH, 'r');
  PrintAccessStatus(&fstat, S_IWOTH, 'w');
  PrintAccessStatus(&fstat, S_IXOTH, 'x');

  PrintLinkCount(&fstat);

  PrintOwnerAndGroup(&fstat);

  PrintSize(&fstat);

  PrintLastModified(&fstat);

  printf("%s\n", path);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Argument is not provided");
    return 1;
  }

  PrintFileStat(argv[1]);

  return 0;
}

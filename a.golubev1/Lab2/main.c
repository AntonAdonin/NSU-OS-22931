#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
  char* pst_timezone = "PST8PDT";
  setenv("TZ", pst_timezone, 1);
  tzset();

  time_t now;
  struct tm* sp;

  time(&now);

  printf("%s", ctime(&now));

  sp = localtime(&now);
  printf("%d/%d/%02d %d:%02d %s\n",
         sp->tm_mon + 1, sp->tm_mday,
         sp->tm_year, sp->tm_hour,
         sp->tm_min, tzname[sp->tm_isdst]);
  exit(0);
}
#define _CRT_SECURE_NO_WARNINGS
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define DIFF_HOURS 7

extern char* tzname[];

main() {
    time_t now;
    struct tm* sp;
    (void)time(&now);
    sp = gmtime(&now);

    sp->tm_hour -= DIFF_HOURS;
    time_t next = mktime(sp);
    sp = localtime(&next);
    printf("%d/%d/%02d %d:%02d\n",
        sp->tm_mon + 1, sp->tm_mday,
        sp->tm_year, sp->tm_hour,
        sp->tm_min);
    exit(0);
    return 0;
}
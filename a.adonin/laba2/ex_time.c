#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

int main(){
    time_t now;
    struct tm *sp;
    (void) time( &now );
	setenv("TZ", "America/Los_Angeles", 1);
	tzset();

	sp = localtime(&now);
	printf("%d/%d/%d %02d:%02d Califonia %s\n",
    sp->tm_mon + 1, sp->tm_mday,
    1900 + sp->tm_year, sp->tm_hour,
    sp->tm_min, tzname[sp->tm_isdst]);
    exit(0);
}
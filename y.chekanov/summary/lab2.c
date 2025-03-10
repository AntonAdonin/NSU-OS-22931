#define _CRT_SECURE_NO_WARNINGS
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char* tzname[];

int main(){
	time_t now;
	struct tm* sp;
	(void)time(&now);
	putenv("TZ=US/Pacific");
	
	now = time(NULL);
	printf("%s", ctime(&now));

	sp = localtime(&now);
	printf("%d/%d/%02d %d:%02d %s\n",
		sp->tm_mon + 1, sp->tm_mday,
		sp->tm_year+1900, sp->tm_hour,
		sp->tm_min, tzname[sp->tm_isdst]);

	return 0;
}
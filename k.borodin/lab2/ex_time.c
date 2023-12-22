#include <stdio.h>
#include <time.h>
#include <stdlib.h>
extern char *tzname[];

void print_time(){
	time_t now;
        struct tm *sp;
	setenv("TZ", "America/Los_Angeles", 1);
        time(&now);
        sp = localtime(&now);
 
        printf("%02d\\%02d\\%.2d %02d:%02d %s\n",
                sp->tm_mon + 1, sp->tm_mday,
                sp->tm_year - 100, sp->tm_hour,
                sp->tm_min, tzname[sp->tm_isdst]);	
}

int main() {
	print_time();
	return 0;
}

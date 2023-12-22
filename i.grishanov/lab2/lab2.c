#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static void tz_time(char* tz, time_t t0)
{
    putenv(tz);
    tzset();
    struct tm *u = localtime(&t0);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S, %A", u);
    printf("%-24s: %s\n", tz, buffer);
}

main(int argc, char **argv)
{
    time_t t0 = time(0);
    tz_time("TZ=US/Pacific", t0);

    return(0);
}
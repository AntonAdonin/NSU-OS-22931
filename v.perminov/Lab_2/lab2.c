#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    struct tm *today;

    time_t current_time = time(NULL);
    today = gmtime(&current_time);

    // Check for the DST
    if (today->tm_isdst) {
        today->tm_hour -= 7;
    } else {
        today->tm_hour -= 8;
    }

    // Check for the early morning time by UTC
    if (today->tm_hour < 0) {
        today->tm_hour += 24;
        today->tm_mday--;
    }

    // Print the current time in California
    printf("\nCurrent date and time in California:\n%02d/%02d/%d %02d:%02d:%02d\n\n",
           today->tm_mday, today->tm_mon + 1, today->tm_year + 1900,
           today->tm_hour, today->tm_min, today->tm_sec);

    return 0;
}

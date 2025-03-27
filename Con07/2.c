#include <stdio.h>
#include <time.h>

enum
{
    LOW_BORDER = 1910,
    UPPER_BORDER = 2037,
    BEGIN = 1900,
    YEAR = 366,
    THURSDAY = 4,
    SECOND = 2,
    FOURTH = 4,
    DIVISION = 3,
    WEEK = 7
};

int
main(void)
{
    int year;
    if (scanf("%d", &year) != 1) {
        return 1;
    }
    if (year < LOW_BORDER || year > UPPER_BORDER) {
        return 1;
    }

    struct tm cur = {.tm_year = year - BEGIN, .tm_mon = 0, .tm_mday = 1};
    if (mktime(&cur) == -1) {
        return 1;
    }

    // ищем четверг
    while (cur.tm_wday != THURSDAY) {
        cur.tm_mday++;
        if (mktime(&cur) == -1) {
            return 1;
        }
    }

    int i = cur.tm_yday;
    int count = 0;
    int prev = 0;

    while (i <= YEAR) {
        if (cur.tm_mon == prev + 1) {
            count = 0;
            prev = cur.tm_mon;
        }

        count++;
        if (count == SECOND || count == FOURTH) {
            if (cur.tm_mday % DIVISION != 0) {
                printf("%d %d\n", cur.tm_mon + 1, cur.tm_mday);
            }
        }

        cur.tm_mday += WEEK;
        if (mktime(&cur) == -1) {
            return 1;
        }
        i++;
    }

    return 0;
}
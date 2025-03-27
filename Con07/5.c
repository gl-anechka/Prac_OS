#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum
{
    LOW_BORDER = 1902,
    UPPER_BORDER = 2037,
    BEGIN = 1900,
    SECONDS = 60,
    HOURS = 24,
    EX_YEAR = 2021 - BEGIN,
    EX_MON = 4,
    EX_DAY = 26,
    EX_HOUR = 11,
    EX_MIN = 14,
    FOURTH = 4,
    DAY_AFTER = 256,
    PERIOD = 29 * HOURS * SECONDS * SECONDS + 12 * SECONDS * SECONDS + 44 * SECONDS
};

void
print_date(int year, int month, int day)
{
    if (month < 10 && day < 10) {
        printf("%d-0%d-0%d\n", year, month, day);
    } else if (month < 10) {
        printf("%d-0%d-%d\n", year, month, day);
    } else if (day < 10) {
        printf("%d-%d-0%d\n", year, month, day);
    } else {
        printf("%d-%d-%d\n", year, month, day);
    }
}

void
add_or_sub_sec(struct tm *cur, int value, int flag)
{
    if (flag == 1) {
        cur->tm_sec += value;
        if (mktime(cur) == -1) {
            exit(1);
        }
    } else {
        cur->tm_sec -= value;
        if (mktime(cur) == -1) {
            exit(1);
        }
    }
}

int
main(void)
{
    int year;
    scanf("%d", &year);
    if (year < LOW_BORDER || year > UPPER_BORDER) {
        return 1;
    }
    year -= BEGIN;

    // пример, когда было полнолуние
    struct tm full_moon = {0, EX_MIN, EX_HOUR, EX_DAY, EX_MON, EX_YEAR, 0, 0, 0};
    if (mktime(&full_moon) == -1) {
        return 1;
    }

    int moon_period = PERIOD;

    // полнолуние после 256 дня
    if (year >= EX_YEAR) {
        while (full_moon.tm_year != year) {
            add_or_sub_sec(&full_moon, moon_period, 1);
        }

        while (full_moon.tm_yday < DAY_AFTER) {
            add_or_sub_sec(&full_moon, moon_period, 1);
        }
    } else {
        while (full_moon.tm_year != year) {
            add_or_sub_sec(&full_moon, moon_period, -1);
        }

        while (full_moon.tm_yday > DAY_AFTER - 1) {
            add_or_sub_sec(&full_moon, moon_period, -1);
        }
        add_or_sub_sec(&full_moon, moon_period, 1);
    }

    // 4-й понедельник
    int count = 0;
    while (count < FOURTH) {
        full_moon.tm_mday++;
        if (mktime(&full_moon) == -1) {
            return 1;
        }
        if (full_moon.tm_wday == 1) {
            count++;
        }
    }
    print_date(full_moon.tm_year + BEGIN, full_moon.tm_mon + 1, full_moon.tm_mday);
    return 0;
}
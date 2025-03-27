#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    PERCENT_100 = 100, // пересчет процентов в дроби
    ACCURACY = 10000 // точность при округлении числа
};

int
main(int argc, char *argv[])
{
    char *eptr = NULL;
    double curr = strtod(argv[1], &eptr);
    curr *= ACCURACY;

    for (int i = 2; i < argc; i++) {
        double percent = strtod(argv[i], &eptr);
        curr *= (1.0 + percent / PERCENT_100);
        curr = round(curr);
    }

    printf("%.4lf\n", curr / ACCURACY);

    return 0;
}
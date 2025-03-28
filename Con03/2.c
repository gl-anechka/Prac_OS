#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    long long sum_pos = 0;
    long long sum_neg = 0;

    for (int i = 1; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        long value = strtol(argv[i], &eptr, 10);

        if (errno || *eptr || eptr == argv[i] || (int) value != value) {
            return 1;
        }

        if (value > 0) {
            sum_pos += value;
        } else {
            sum_neg += value;
        }
    }

    printf("%lld\n%lld\n", sum_pos, sum_neg);

    return 0;
}
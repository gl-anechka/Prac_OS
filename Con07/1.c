#include <stdio.h>
#include <ctype.h>
#include <limits.h>

enum
{
    BASE = 3
};

const char str_overflow[] = "18446744073709551616";
const char alphabet[] = "01a";

int
main(void)
{
    char elem;
    long long res = 0;
    int check = 0;
    int check_min = 0;
    int not_valid = 0;

    while ((elem = getchar()) != EOF) {
        if (isspace(elem) > 0) {
            check = 0;
            continue;
        }

        if (check > 0) {
            continue;
        }

        check = 0;

        while ((elem != EOF) && !isspace(elem)) {
            if (elem != alphabet[0] && elem != alphabet[1] && elem != alphabet[2]) {
                not_valid = 1;
                return 1;
            }

            if (__builtin_mul_overflow(res, BASE, &res)) {
                if (res == LLONG_MAX && elem == alphabet[1]) {
                    printf("%lld\n", LLONG_MIN);
                    check_min = 1;
                    break;
                }
                check = 1;
                break;
            }

            if (elem == alphabet[2]) {
                if (__builtin_ssubll_overflow(res, 1, &res)) {
                    check = 1;
                    break;
                }
            } else if (elem == alphabet[1]) {
                if (__builtin_saddll_overflow(res, 1, &res)) {
                    check = 1;
                    break;
                }
            }

            elem = getchar();
        }

        if (!check) {
            if (!check_min) {
                if (!not_valid) {
                    printf("%lld\n", res);
                }
            } else {
                check_min = 0;
            }

            if (!not_valid) {
                res = 0;
            } else {
                not_valid = 0;
            }
        } else {
            printf("%s\n", str_overflow);
            res = 0;
        }
    }

    return 0;
}

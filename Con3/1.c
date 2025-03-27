#include <stdio.h>

typedef int STYPE;
typedef unsigned int UTYPE;

STYPE
bit_reverse(STYPE value)
{
    UTYPE temp = (UTYPE) value | ~((UTYPE) value);
    UTYPE res = 0;
    UTYPE mask = (UTYPE) value;

    while (temp != 0) {
        temp = temp >> 1;

        res = res << 1;
        res = res | (mask & 1);
        mask = mask >> 1;
    }

    return (STYPE) res;
}

int
main(void)
{
    int n;

    for (int i = 0; i < 10; i++) {
        scanf("%d", &n);
        printf("new = %d\told = %d\n", bit_reverse(n), bit_reverse_old(n));
    }

    return 0;
}
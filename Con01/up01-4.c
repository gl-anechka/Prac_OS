#include <stdio.h>
#include <stdlib.h>

int
compare(const void *A, const void *B)
{
    int a = *((int *) A);
    int b = *((int *) B);

    if (!(a & 1) && !(b & 1)) {
        return (a > b) - (a < b);
    } else if ((a & 1) && (b & 1)) {
        return (b > a) - (b < a);
    } else {
        if (!(a & 1) && (b & 1)) {
            return -1;
        } else {
            return 1;
        }
    }
}

void
sort_even_odd(size_t count, int *data)
{
    if (count <= 0) {
        return;
    }

    qsort(data, count, sizeof(*data), compare);
}

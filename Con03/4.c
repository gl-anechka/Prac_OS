#include <stdio.h>

enum
{
    MY_INT_MAX = ((unsigned) ~0) >> (-(~0)),
    MY_INT_MIN = ~(MY_INT_MAX)
};

int
satsum(int v1, int v2)
{
    if (v1 > 0) {
        if (v2 > MY_INT_MAX - v1) {
            return MY_INT_MAX;
        }
    } else if (v1 < 0) {
        if (v2 < MY_INT_MIN - v1) {
            return MY_INT_MIN;
        }
    } else if (v2 > 0) {
        if (v1 > MY_INT_MAX - v2) {
            return MY_INT_MAX;
        }
    } else if (v2 < 0) {
        if (v1 < MY_INT_MIN - v2) {
            return MY_INT_MIN;
        }
    }

    return v1 + v2;
}
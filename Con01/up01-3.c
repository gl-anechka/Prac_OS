#include <stdio.h>
#include <stdlib.h>

enum
{
    LOW_BORDER = 1,
    UPPER_BORDER = 2000
};

int
main()
{
    int N;
    if (scanf("%d", &N) != 1) {
        return 1;
    }

    if (N <= LOW_BORDER || N >= UPPER_BORDER) {
        return 1;
    }

    int *array = calloc(N, sizeof(*array));

    if (array == NULL) {
        return 1;
    }

    for (int i = 1; i < N; i++) {
        for (int j = 1; j < N; j++) {
            if ((i * j) % N == 1) {
                array[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 1; j < N; j++) {
            printf("%d ", (array[j] * i) % N);
        }
        printf("\n");
    }

    free(array);
    return 0;
}

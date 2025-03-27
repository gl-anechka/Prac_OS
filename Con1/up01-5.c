#include <stdio.h>
#include <stdlib.h>

enum
{
    LOW_BORDER = 0,
    UP_BORDER = 10
};

void
swap(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void
reverse(int *arr, int i, int len)
{
    int left = i;
    int right = len - 1;

    while (left <= right) {
        swap(arr, left, right);
        left++;
        right--;
    }
}

int
next_permutation(int *arr, int len)
{
    int res = 0;
    int i = len - 2;

    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }

    if (i >= 0) {
        int k = len - 1;
        while (arr[k] <= arr[i]) {
            k--;
        }
        swap(arr, i, k);
        res = 1;
    }

    reverse(arr, i + 1, len);

    return res;
}

int
main(void)
{
    int N;
    if (scanf("%d", &N) != 1) {
        return 1;
    }
    if (N <= LOW_BORDER || N >= UP_BORDER) {
        return 1;
    }

    int *array = calloc(N, sizeof(*array));
    if (array == NULL) {
        return 1;
    }

    for (int i = 0; i < N; i++) {
        array[i] = i + 1;
    }

    do {
        for (int i = 0; i < N; i++) {
            printf("%d", array[i]);
        }
        printf("\n");
    } while (next_permutation(array, N));

    free(array);

    return 0;
}

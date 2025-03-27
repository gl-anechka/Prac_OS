#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct s1
{
    char f1;
    long long f2;
    char f3;
};

struct s2
{
    long long f2;
    char f1;
    char f3;
};

size_t
compactify(void *ptr, size_t size)
{
    if (size == 0) {
        return 0;
    }

    size_t num_of_elem = size / sizeof(struct s1);
    size_t new_size = num_of_elem * sizeof(struct s2);
    struct s2 *new_ptr = calloc(num_of_elem, sizeof(struct s2));
    if (new_ptr == NULL) {
        return 0;
    }
    struct s1 *old_ptr = (struct s1 *) ptr;

    for (size_t i = 0; i < num_of_elem; i++) {
        new_ptr[i].f1 = old_ptr[i].f1;
        new_ptr[i].f2 = old_ptr[i].f2;
        new_ptr[i].f3 = old_ptr[i].f3;
    }
    memcpy(ptr, new_ptr, new_size);
    free(new_ptr);

    return new_size;
}

int
main()
{
    printf("%d %d\n", sizeof(struct s1), sizeof(struct s2));

    struct s1 *head = (struct s1*) calloc(2, sizeof(struct s1));

    head[0].f1 = 1;
    head[0].f2 = 2;
    head[0].f3 = 3;

    head[1].f1 = 4;
    head[1].f2 = 5;
    head[1].f3 = 6;

    printf("old struct:\n");
    printf("%d %lld %d\n%d %lld %d\n", head[0].f1, head[0].f2, head[0].f3, head[1].f1, head[1].f2, head[1].f3);

    size_t size2 = compactify((void*) head, 2 * sizeof(struct s1));
    printf("old = %d new = %d\n", 2 * sizeof(struct s1), size2);
    printf("new struct:\n");
    printf("%d %lld %d\n%d %lld %d\n", head[0].f1, head[0].f2, head[0].f3, head[1].f1, head[1].f2, head[1].f3);
    return 0;
}
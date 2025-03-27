#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

enum
{
    BASIS = 10,
    MAX_LEN = 12
};

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *
dup_elem(struct Elem *head)
{
    if (!head) {
        return NULL;
    }

    struct Elem *node = head;
    struct Elem *prev = head;

    while (node) {
        char *eptr = NULL;
        errno = 0;
        long value = strtol(node->str, &eptr, BASIS);

        if (!errno && !(*eptr) && eptr != node->str && (int) value == value) {
            int res;
            if (!__builtin_add_overflow(value, 1, &res)) {
                struct Elem *new = calloc(1, sizeof(*new));
                if (new == NULL) {
                    fprintf(stderr, "memory error");
                    return NULL;
                }

                if (node == head) {
                    new->next = node;
                    head = new;
                } else {
                    new->next = node;
                    prev->next = new;
                }

                new->str = calloc(MAX_LEN, sizeof(new->str));
                if (new->str == NULL) {
                    fprintf(stderr, "memory error");
                    return NULL;
                }
                snprintf(new->str, MAX_LEN, "%ld", value + 1);
            }
        }

        prev = node;
        node = node->next;
    }

    return head;
}

int
main (void)
{
    struct Elem *head = NULL;
    head = malloc(sizeof(*head));
    head->str = malloc(MAX_LEN);
    snprintf(head->str, MAX_LEN, "%ld", 10);

    head->next = malloc(sizeof(*head));
    head->next->str = malloc(MAX_LEN);
    snprintf(head->next->str, MAX_LEN, "%ld", 3);
    head->next->next = NULL;

    head = dup_elem(head);
    printf("%s\n%s\n%s\n%s\n", head->str, head->next->str, head->next->next->str, head->next->next->next->str);
    return 0;
}

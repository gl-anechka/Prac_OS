#include <stdio.h>
#include <stdint.h>
#include <sys/file.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

enum
{
    SIZE_STRUCT = 12
};

void
print_keys(int file, int32_t ind)
{
    lseek(file, ind, SEEK_SET);
    struct Node *head = calloc(1, SIZE_STRUCT);
    if (head == NULL) {
        return;
    }
    if (read(file, head, sizeof(*head)) != SIZE_STRUCT) {
        return;
    }

    int32_t left = head->left_idx;
    int32_t right = head->right_idx;

    if (right != 0) {
        print_keys(file, right * sizeof(*head));
    }

    printf("%d\n", head->key);

    if (left != 0) {
        print_keys(file, left * sizeof(*head));
    }
    free(head);
}

int
main(int argc, char *argv[])
{
    int file = open(argv[1], O_RDONLY);
    if (file < 0) {
        return 1;
    }

    print_keys(file, 0);

    if (close(file) < 0) {
        return 1;
    }
    return 0;
}
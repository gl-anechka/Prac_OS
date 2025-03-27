#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char full_rights[] = "rwxrwxrwx"; // полные права на файл

enum
{
    BASIS = 8,
    LEN_CONST = sizeof(full_rights) - 1
};

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        long value = strtol(argv[i], &eptr, BASIS);

        if (errno || *eptr || eptr == argv[i] || (int) value != value) {
            return 1;
        }

        unsigned long mask = 1 << BASIS; // проверка присутствия бита в числе
        for (int j = 0; j < LEN_CONST; j++) {
            if (!((unsigned long) value & mask)) {
                printf("-");
            } else {
                printf("%c", full_rights[j]);
            }
            mask = mask >> 1;
        }
        printf("\n");
    }

    return 0;
}
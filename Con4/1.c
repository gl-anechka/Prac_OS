#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    BUF_SIZE = 4,
    MASK1 = 0xf,
    MASK2 = 0xff,
    SHIFT1 = 8,
    SHIFT2 = 4
};

int
main(int argc, char *argv[])
{
    int file = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (file < 0) {
        return 1;
    }

    unsigned value;
    while (scanf("%u", &value) == 1) {
        char numbers[BUF_SIZE];

        numbers[3] = value & MASK2;
        value = value >> SHIFT1;
        numbers[2] = value & MASK1;
        value = value >> SHIFT2;

        numbers[1] = value & MASK2;
        value = value >> SHIFT1;
        numbers[0] = value & MASK1;
        value = value >> SHIFT2;

        if (write(file, numbers, sizeof(numbers)) < 0) {
            return 1;
        }
    }

    if (close(file) < 0) {
        return 1;
    }

    return 0;
}
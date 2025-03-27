#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

enum
{
    BASIS = 10
};

int
main(int argc, char *argv[])
{
    int file1 = open(argv[1], O_RDONLY);
    if (file1 < 0) {
        return 1;
    }
    int file2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (file2 < 0) {
        return 1;
    }

    errno = 0;
    char *end = NULL;
    long mod = strtol(argv[3], &end, BASIS);
    if (errno || *end || end == argv[3] || (int) mod != mod) {
        return 1;
    }

    unsigned char value;
    unsigned long long sum = 0;
    unsigned long long count = 0;
    int value_print = 0;

    while (read(file1, &value, sizeof(value)) > 0) {
        for (int i = 0; i < CHAR_BIT; i++) {
            count++;
            sum += count * count;

            if ((value & ((unsigned char) 1)) == 1) {
                value_print = sum % mod;
                if (write(file2, &value_print, sizeof(value_print)) < 0) {
                    return 1;
                }
            }
            value = value >> 1;
        }
    }

    if (close(file1) < 0) {
        return 1;
    }
    if (close(file2) < 0) {
        return 1;
    }
    return 0;
}

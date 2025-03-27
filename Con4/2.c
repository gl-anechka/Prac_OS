#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

enum
{
    BASIS = 10
};

int
main(int argc, char *argv[])
{
    int file = open(argv[1], O_RDWR);
    if (file < 0) {
        return 1;
    }

    char *eptr = NULL;
    errno = 0;
    long N = strtol(argv[2], &eptr, BASIS);

    if (*eptr || errno || eptr == argv[2] || (int) N != N) {
        return 1;
    }

    double number;
    if (read(file, &number, sizeof(number)) < 0) {
        return 1;
    }

    int count = 1;
    double cur;
    while (read(file, &cur, sizeof(cur)) == sizeof(cur) && count < N) {
        if (lseek(file, -sizeof(cur), SEEK_CUR) < 0) {
            return 1;
        }
        number = cur - number;
        if (write(file, &number, sizeof(number)) < 0) {
            return 1;
        }
        count++;
    }

    if (close(file) < 0) {
        return 1;
    }

    return 0;
}
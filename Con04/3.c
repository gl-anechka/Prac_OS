#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    int file = open(argv[1], O_RDWR);
    if (file < 0) {
        return 1;
    }

    long long value;
    if (read(file, &value, sizeof(value)) != sizeof(value)) {
        return 0;
    }

    int ind = 0;
    long long min_value = value;

    while (read(file, &value, sizeof(value)) == sizeof(value)) {
        if (value < min_value) {
            min_value = value;
            if ((ind = lseek(file, 0, SEEK_CUR) - sizeof(value)) < 0) {
                return 1;
            } // позиция нового минимального числа
        }
    }

    if (min_value > LLONG_MIN) {
        if (lseek(file, ind, SEEK_SET) < 0) {
            return 1;
        }
        min_value = -min_value;
        if (write(file, &min_value, sizeof(min_value)) < 0) {
            return 1;
        }
    }

    if (close(file) < 0) {
        return 1;
    }
    return 0;
}
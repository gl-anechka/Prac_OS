#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        return 1;
    }
    int file = open(argv[1], O_RDWR);
    if (file < 0) {
        return 1;
    }

    int mask_len = lseek(file, 0, SEEK_END) * CHAR_BIT;
    if (mask_len < 0) {
        return 1;
    }

    int x;
    char buf;
    while (scanf("%d", &x) == 1) {
        if (x > 0 && x <= mask_len) {
            x--;
            if (lseek(file, x / CHAR_BIT, SEEK_SET) < 0) {
                return 1;
            }
            if (read(file, &buf, sizeof(buf)) < 0) {
                return 1;
            }
            buf |= (1 << x % CHAR_BIT);
            if (lseek(file, x / CHAR_BIT, SEEK_SET) < 0) {
                return 1;
            }
            if (write(file, &buf, sizeof(buf)) < 0) {
                return 1;
            }
        }
        if (x < 0 && -x <= mask_len) {
            x = -x - 1;
            if (lseek(file, x / CHAR_BIT, SEEK_SET) < 0) {
                return 1;
            }
            if (read(file, &buf, sizeof(buf)) < 0) {
                return 1;
            }
            buf &= ~(1 << x % CHAR_BIT);
            if (lseek(file, x / CHAR_BIT, SEEK_SET) < 0) {
                return 1;
            }
            if (write(file, &buf, sizeof(buf)) < 0) {
                return 1;
            }
        }
    }

    if (close(file) < 0) {
        return 1;
    }
    return 0;
}

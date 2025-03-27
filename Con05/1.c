#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

enum
{
    DIVISION = 1024 // проверка на целое число кибибайт
};

int
main(int argc, char *argv[])
{
    long long count = 0;

    for (int i = 1; i < argc; i++) {
        struct stat stb;

        if (lstat(argv[i], &stb) == 0) {
            if (S_ISREG(stb.st_mode)) {
                if (stb.st_nlink == 1 && stb.st_size % DIVISION == 0) {
                    count += stb.st_size;
                }
            }
        }
    }

    printf("%lld\n", count);

    return 0;
}

#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

const char suffix[] = ".exe";

enum
{
    LEN_OF_SUFFIX = sizeof(suffix) - 1
};

int
main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    if (d == NULL) {
        return 1;
    }

    struct dirent *dd;
    unsigned long long count = 0;

    while ((dd = readdir(d)) != NULL) {
        char name[PATH_MAX];
        struct stat file;
        if (snprintf(name, sizeof(name), "%s/%s", argv[1], dd->d_name) < 0) {
            continue;
        }

        if (stat(name, &file) >= 0) {
            if (S_ISREG(file.st_mode) && (access(name, X_OK) == 0)) {
                unsigned int len = strlen(dd->d_name);
                if ((len >= LEN_OF_SUFFIX) && !(strcmp(dd->d_name + len - LEN_OF_SUFFIX, suffix))) {
                    count++;
                }
            }
        }
    }

    if (closedir(d) < 0) {
        return 1;
    }

    printf("%llu\n", count);
    return 0;
}
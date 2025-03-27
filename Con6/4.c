#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
    BASIS = 10
};

void
files(char *dir, int size, int depth, int path_len)
{
    if (depth > 4) {
        return;
    }

    DIR *d = opendir(dir);
    if (d == NULL) {
        return;
    }

    struct dirent *dd = NULL;
    while ((dd = readdir(d)) != NULL) {
        if (strcmp(dd->d_name, ".") != 0 && strcmp(dd->d_name, "..") != 0) {
            char name[PATH_MAX];
            if (snprintf(name, sizeof(name), "%s/%s", dir, dd->d_name) >= 0) {
                struct stat file;
                if (lstat(name, &file) >= 0) {
                    if (S_ISDIR(file.st_mode)) {
                        files(name, size, depth + 1, path_len);
                    } else if (S_ISREG(file.st_mode) && file.st_size <= size && (!access(name, R_OK))) {
                        printf("%s\n", name + path_len);
                    }
                }
            }
        }
    }

    if (closedir(d) < 0) {
        return;
    }
}

int
main(int argc, char *argv[])
{
    char *end = NULL;
    errno = 0;
    long max_size = strtol(argv[2], &end, BASIS);
    if (errno || *end || end == argv[2] || (int) max_size != max_size) {
        return 1;
    }

    files(argv[1], max_size, 1, strlen(argv[1]) + 1);

    return 0;
}

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    DIR *d1 = opendir(argv[1]);
    if (d1 == NULL) {
        return 1;
    }

    DIR *d2 = opendir(argv[2]);
    if (d2 == NULL) {
        return 1;
    }

    struct dirent *dd1;
    struct dirent *dd2;
    unsigned long long count = 0;

    while ((dd1 = readdir(d1)) != NULL) {
        char name1[PATH_MAX];
        struct stat file1;
        if (snprintf(name1, sizeof(name1), "%s/%s", argv[1], dd1->d_name) < 0) {
            return 1;
        }

        if (lstat(name1, &file1) < 0) {
            return 1;
        }

        if (S_ISREG(file1.st_mode) && (access(name1, W_OK) == 0)) {
            while ((dd2 = readdir(d2)) != NULL) {
                char name2[PATH_MAX];
                struct stat file2;
                if (snprintf(name2, sizeof(name2), "%s/%s", argv[2], dd2->d_name) < 0) {
                    return 1;
                }

                if (stat(name2, &file2) < 0) {
                    return 1;
                }

                if (!strcmp(dd1->d_name, dd2->d_name)) {
                    if ((file1.st_dev == file2.st_dev) && (file1.st_ino == file2.st_ino)) {
                        count += file1.st_size;
                    }
                }
            }
            seekdir(d2, 0);
        }
    }

    if (closedir(d1) < 0) {
        return 1;
    }
    if (closedir(d2) < 0) {
        return 1;
    }

    printf("%llu\n", count);
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <limits.h>

int
comp(const void *a, const void *b)
{
    char *A = (char *) a;
    char *B = (char *) b;
    return strcmp(A, B);
}

int
main(int argc, char *argv[])
{
    char **files = calloc(argc, sizeof(*files));
    if (*files == NULL) {
        return 1;
    }

    int count_files = 0;
    struct stat buf;

    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &buf) > 0) {
            for (int j = 0; j < count_files; j++) {
                struct stat check;
                int check_val = 0;

                if (stat(files[j], &check) > 0) {
                    if ((check.st_dev == buf.st_dev) && (check.st_ino == buf.st_ino)) {
                        check_val = 1;
                        if (strcmp(files[j], argv[i]) > 0) {
                            files[j] = argv[i];
                        }
                    }
                    break;
                }
                if (check_val == 0) {
                    files[count_files++] = argv[i];
                }
            }
        }
    }

    qsort(files, count_files, sizeof(*files), comp);

    for (int i = 0; i < count_files; ++i) {
        printf("%s\n", files[i]);
    }

    free(files);

    return 0;
}
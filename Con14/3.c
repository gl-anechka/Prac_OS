#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <wait.h>
#include <errno.h>

enum
{
    BASIS = 10
};

char path[PATH_MAX];

void
read_file(char *st)
{
    FILE *f = fopen(st, "r");
    if (f == NULL) {
        _exit(1);
    }
    if (fgets(path, sizeof(path), f) == NULL) {
        _exit(1);
    }
    path[strlen(path) - 1] = '\0';
    fclose(f);

    if (execlp(path, path, NULL) < 0) {
        _exit(1);
    }
}

int
main(int argc, char *argv[])
{
    errno = 0;
    char *end = NULL;
    long n = strtol(argv[1], &end, BASIS);
    if (errno || *end || end == argv[1] || (int) n != n) {
        return 1;
    }

    pid_t pid;
    int status;
    long long count = 0;

    for (int i = 2; (i < n + 2) && (i < argc); i++) {
        if ((pid = fork()) < 0) {
            return 1;
        } else if (pid == 0) {
            read_file(argv[i]);
        }
    }

    while (wait(&status) > 0) {
        count += (WIFEXITED(status) && !WEXITSTATUS(status));
    }

    for (int i = n + 2; i < argc; i++) {
        if ((pid = fork()) < 0) {
            return 1;
        } else if (pid == 0) {
            read_file(argv[i]);
        } else {
            wait(&status);
            count += (WIFEXITED(status) && !WEXITSTATUS(status));
        }
    }

    printf("%lld\n", count);
    fflush(stdout);
    return 0;
}
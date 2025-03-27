#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int
main(void)
{
    pid_t pid;
    int fd[2];
    if (pipe(fd) < 0) {
        return 1;
    }

    if ((pid = fork()) < 0) {
        return 1;
    } else if (pid == 0) {
        if ((pid = fork()) < 0) {
            _exit(1);
        } else if (pid == 0) {
            if (close(fd[1]) < 0) {
                _exit(1);
            }

            long long res = 0;
            int elem;
            while (read(fd[0], &elem, sizeof(elem)) == sizeof(elem)) {
                res += elem;
            }

            printf("%lld\n", res);
            fflush(stdout);
            _exit(0);
        }
        if (close(fd[0]) < 0) {
            _exit(1);
        }
        if (close(fd[1]) < 0) {
            _exit(1);
        }

        wait(NULL);
        _exit(0);
    }

    if (close(fd[0]) < 0) {
        return 1;
    }

    int value;
    while (scanf("%d", &value) == 1) {
        if (write(fd[1], &value, sizeof(value)) < 0) {
            return 1;
        }
    }

    if (close(fd[1]) < 0) {
        return 1;
    }

    wait(NULL);
    return 0;
}
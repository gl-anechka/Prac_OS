#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

enum
{
    YEAR = 1900,
    REPEAT = 3  //т.к. читаем из канала 3 раза
};

void
read_from_pipe(int *fd, char data)
{
    if (close(fd[1]) < 0) {
        _exit(1);
    }

    time_t cur;
    if (read(fd[0], &cur, sizeof(cur)) < 0) {
        _exit(1);
    }
    struct tm *res;
    res = localtime(&cur);

    if (data == 'D') {
        printf("D:%.02d\n", res->tm_mday);
    } else if (data == 'M') {
        printf("M:%.02d\n", res->tm_mon + 1);
    } else if (data == 'Y') {
        printf("Y:%d\n", res->tm_year + YEAR);
    }
    fflush(stdout);

    if (close(fd[0]) < 0) {
        _exit(1);
    }
}

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
            if ((pid = fork()) < 0) {
                _exit(1);
            } else if (pid == 0) {
                time_t now = time(NULL);
                if (close(fd[0]) < 0) {
                    _exit(1);
                }

                for (int i = 0; i < REPEAT; i++) {
                    if (write(fd[1], &now, sizeof(now)) < 0) {
                        _exit(1);
                    }
                }

                if (close(fd[1]) < 0) {
                    _exit(1);
                }
                _exit(0);
            }
            wait(NULL);
            read_from_pipe(fd, 'D');
            _exit(0);
        }
        wait(NULL);
        read_from_pipe(fd, 'M');
        _exit(0);
    }

    wait(NULL);
    read_from_pipe(fd, 'Y');

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>

enum
{
    BASIS = 10,
    PID1 = 1,
    PID2 = 2
};

void
child_process(int fd0, int fd1, long n, int pid)
{
    if (close(fd1) < 0) {
        _exit(1);
    }
    if (close(fd0) < 0) {
        _exit(1);
    }

    // Ввод
    FILE *f_read = fdopen(fd0, "r");
    // Вывод
    FILE *f_write = fdopen(fd1, "w");

    int num;
    while (fscanf(f_read, "%d", &num) == 1) {
        if (num < n) {
            printf("%d %d\n", pid, num);
            fflush(stdout);
            fprintf(f_write, "%d ", num + 1);
            fflush(f_write);
        } else {
            fclose(f_write);
            fclose(f_read);
            _exit(0);
        }
    }
    fclose(f_write);
    fclose(f_read);
    _exit(0);
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

    int fd1[2];
    int fd2[2];

    pipe(fd1);
    pipe(fd2);

    //child1 --> fd2 --> child2
    //child1 <-- fd1 <-- child2

    //число "1" для первого сына
    FILE *f = fdopen(fd1[1], "w");
    fprintf(f, "1 ");
    fflush(f);

    pid_t pid1;
    pid_t pid2;

    if ((pid1 = fork()) < 0) {
        return 1;
    } else if (pid1 == 0) {
        child_process(fd1[0], fd2[1], n, PID1);
        /*if (close(fd1[1]) < 0) {
            _exit(1);
        }
        if (close(fd2[0]) < 0) {
            _exit(1);
        }

        //ввод
        FILE *f_read = fdopen(fd1[0], "r");
        //вывод
        FILE *f_write = fdopen(fd2[1], "w");

        int num;
        while (fscanf(f_read, "%d", &num) == 1) {
            if (num < n) {
                printf("%d %d\n", PID1, num);
                fflush(stdout);
                fprintf(f_write, "%d ", num + 1);
                fflush(f_write);
            } else {
                fclose(f_write);
                fclose(f_read);
                _exit(0);
            }
        }
        fclose(f_write);
        fclose(f_read);
        _exit(0);*/
    }

    if ((pid2 = fork()) < 0) {
        return 1;
    } else if (pid2 == 0) {
        child_process(fd2[0], fd1[1], n, PID2);
        /*if (close(fd2[1]) < 0) {
            _exit(1);
        }
        if (close(fd1[0]) < 0) {
            _exit(1);
        }

        //ввод
        FILE *f_read = fdopen(fd2[0], "r");
        //вывод
        FILE *f_write = fdopen(fd1[1], "w");

        int num;
        while (fscanf(f_read, "%d", &num) == 1) {
            if (num < n) {
                printf("%d %d\n", PID2, num);
                fflush(stdout);
                fprintf(f_write, "%d ", num + 1);
                fflush(f_write);
            } else {
                fclose(f_write);
                fclose(f_read);
                _exit(0);
            }
        }
        fclose(f_write);
        fclose(f_read);
        _exit(0);*/
    }

    if (close(fd1[0]) < 0) {
        return 1;
    }
    if (close(fd1[1]) < 0) {
        return 1;
    }
    if (close(fd2[0]) < 0) {
        return 1;
    }
    if (close(fd2[1]) < 0) {
        return 1;
    }

    while (wait(NULL) > 0) {
        //wait
    }

    printf("Done\n");
    fflush(stdout);

    return 0;
}

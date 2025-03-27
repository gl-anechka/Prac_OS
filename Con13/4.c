// конвейер

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

void
kill_all(pid_t *pid, int ind)
{
    int status;
    for (int i = 1; i < ind; ++i) {
        kill(pid[i], SIGKILL);
        waitpid(pid[i], &status, 0);
    }
    free(pid);
}

int
main(int argc, char *argv[])
{
    pid_t *pid = calloc(argc, sizeof(*pid));

    if (close(2) < 0) {
        return 1;
    }

    int fd0 = 0;

    for (int i = 1; i < argc; i++) {
        int fd[2];
        if (pipe(fd) < 0) {
            kill_all(pid, i);
            _exit(1);
        }

        if ((pid[i] = fork()) < 0) {
            kill_all(pid, i);
            close(fd[0]);
            close(fd[1]);
            _exit(1);
        } else if (pid[i] == 0) {
            close(fd[0]);

            if (i == 1 && argc != 2) {
                //для первого процесса
                //поток вывода в канал
                dup2(fd[1], 1);
            } else if (i == (argc - 1)) {
                //для последнего процесса
                //поток ввода из предыдущего процесса
                if (fd0 != 0) {
                    dup2(fd0, 0);
                }
            } else {
                //для процесса в середине конвейера
                //ввод из предыдущего процесса, вывод в канал
                dup2(fd[1], 1);
                if (fd0 != 0) {
                    dup2(fd0, 0);
                }
            }
            if (fd0 != 0) {
                close(fd0);
            }
            if (execlp(argv[i], argv[i], NULL) < 0) {
                _exit(1);
            }
        } else {
            close(fd[1]);
            if (fd0 != 0) {
                close(fd0);
            }
            fd0 = fd[0];
        }
    }

    close(fd0);
    free(pid);

    while (wait(NULL) > 0)
    {
        //wait
    }
    return 0;
}
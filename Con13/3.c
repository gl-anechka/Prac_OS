// ( cmd1 < file1 && cmd2 ) | cmd3 >> file2

#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/file.h>

int
main(int argc, char *argv[])
{
    int fds[2];
    pipe(fds);

    pid_t pid;

    if ((pid = fork()) < 0) {
        return 1;
    } else if (pid == 0) {
        if (close(fds[0]) < 0) {
            _exit(1);
        }
        if (dup2(fds[1], 1) < 0) {
            _exit(1);
        }
        if (close(fds[1]) < 0) {
            _exit(1);
        }

        //cmd1
        if ((pid = fork()) < 0) {
            _exit(1);
        } else if (pid == 0) {
            int file_1 = open(argv[4], O_RDONLY);
            if (file_1 < 0) {
                _exit(1);
            }
            if (dup2(file_1, 0) < 0) {
                _exit(1);
            }
            if (close(file_1) < 0) {
                _exit(1);
            }
            if (execlp(argv[1], argv[1], NULL) < 0) {
                _exit(1);
            }
        }

        //если cmd1 отработал без ошибок, выполняем cmd2
        int status;
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if ((pid = fork()) < 0) {
                _exit(1);
            } else if (pid == 0) {
                if (execlp(argv[2], argv[2], NULL) < 0) {
                    _exit(1);
                }
            }
            wait(NULL);
        }
        _exit(0);
    }

    //cmd3
    if ((pid = fork()) < 0) {
        return 1;
    } else if (pid == 0) {
        int file_2 = open(argv[5], O_CREAT | O_WRONLY | O_APPEND, 0700);

        if (file_2 < 0) {
            _exit(1);
        }
        if (dup2(file_2, 1) < 0) {
            _exit(1);
        }
        if (close(file_2) < 0) {
            _exit(1);
        }
        if (close(fds[1]) < 0) {
            _exit(1);
        }
        if (dup2(fds[0], 0) < 0) {
            _exit(1);
        }
        if (close(fds[0]) < 0) {
            _exit(1);
        }
        if (execlp(argv[3], argv[3], NULL) < 0) {
            _exit(1);
        }
    }

    if (close(fds[0]) < 0) {
        return 1;
    }
    if (close(fds[1]) < 0) {
        return 1;
    }

    while (wait(NULL) > 0) {
        //wait
    }

    return 0;
}
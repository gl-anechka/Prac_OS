#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <wait.h>

enum
{
    ERROR_COD = 42
};

int
main(int argc, char *argv[])
{
    pid_t pid;
    int status;
    if ((pid = fork()) < 0) {
        return 1;
    }

    if (pid == 0) {
        //stdin
        int file1 = open(argv[2], O_RDONLY);
        if (file1 < 0) {
            _exit(ERROR_COD);
        }
        if (dup2(file1, 0) < 0) {
            _exit(ERROR_COD);
        }

        //stdout
        int file2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        if (file2 < 0) {
            _exit(ERROR_COD);
        }
        if (dup2(file2, 1) < 0) {
            _exit(ERROR_COD);
        }

        //stderr
        int file3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);
        if (file3 < 0) {
            _exit(ERROR_COD);
        }
        if (dup2(file3, 2) < 0) {
            _exit(ERROR_COD);
        }

        //close
        if (close(file1) < 0) {
            _exit(ERROR_COD);
        }
        if (close(file2) < 0) {
            _exit(ERROR_COD);
        }
        if (close(file3) < 0) {
            _exit(ERROR_COD);
        }

        if (execlp(argv[1], argv[1], NULL) < 0) {
            _exit(ERROR_COD);
        }
    } else {
        wait(&status);
        printf("%d\n", status);
    }
    return 0;
}
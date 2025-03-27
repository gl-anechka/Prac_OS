#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int
main(void)
{
    setbuf(stdin, NULL);
    pid_t pid;
    pid_t ppid = getpid();
    int status;

    int value;
    while (1) {
        if (scanf("%d", &value) <= 0) {
            exit(0); //ввод закончился
        }

        pid = fork();
        if (pid == -1) {
            exit(1); //процесс не создался
        }

        if (pid) {
            wait(&status);
            if ((WIFEXITED(status) != 0) && (WEXITSTATUS(status) == 0)) {
                printf("%d\n", value); //печать значения
                fflush(stdout);
                exit(0);
            } else {
                if (getpid() == ppid) {
                    printf("-1\n"); //-1 в случае ошибки
                    fflush(stdout);
                    exit(0);
                } else {
                    exit(1); //завершаем всех сыновей
                }
            }
        }
    }

    while (wait(&status) > 0) {
        //ожидание завершения всех процессов
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

volatile sig_atomic_t flag;

enum
{
    BASIS = 10,
    PID1 = 1,
    PID2 = 2
};

void
handler(int sig)
{
    if (sig == SIGUSR1) {
        flag = 1;
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

    int fd[2];
    pipe(fd);

    //число "1" для первого сына
    FILE *f_read = fdopen(fd[0], "r");
    FILE *f_write = fdopen(fd[1], "w");

    //блокировка сигнала в родителе
    sigset_t sig;
    sigemptyset(&sig);
    sidaddset(&sig, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sig, NULL);

    sigaction(SIGUSR1, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    pid_t pid1;
    pid_t pid2;

    if ((pid1 = fork()) < 0) {
        fclose(f_read);
        fclose(f_write);
        return 1;
    } else if (pid1 == 0) {
        sigset_t sig_ch;
        sigemptyset(&sig_ch);
        sigaddset(&sig_ch, SIGUSR1);

        pid_t pid;
        fscanf(f_read, "%d", &pid);

        while (1) {
            while (flag == 0) {
                sigsuspend(&sig_ch);
            }
            flag = 0;

            int num;
            fscanf(f_read, "%d", &num);
            if (num < n) {
                printf("%d %d\n", PID1, num);
                fflush(stdout);
            }

            fprintf(f_write, "%d ", ++num);
            fflush(f_write);
            kill(pid, SIGUSR1);

            if (num >= n) {
                fclose(f_read);
                fclose(f_write);
                _exit(0);
            }
        }
    }

    if ((pid2 = fork()) < 0) {
        return 1;
    } else if (pid2 == 0) {
        sigset_t sig_ch;
        sigemptyset(&sig_ch);
        sigaddset(&sig_ch, SIGUSR1);

        pid_t pid;
        fscanf(f_read, "%d", &pid);

        while (1) {
            while (flag == 0) {
                sigsuspend(&sig_ch);
            }
            flag = 0;

            int num;
            fscanf(f_read, "%d", &num);
            if (num < n) {
                printf("%d %d\n", PID2, num);
                fflush(stdout);
            }

            fprintf(f_write, "%d ", ++num);
            fflush(f_write);
            kill(pid1, SIGUSR1);

            if (num >= n) {
                fclose(f_read);
                fclose(f_write);
                _exit(0);
            }
        }
    }

    fprintf(f_write, "%d %d", pid2, 1);
    kill(pid1, SIGUSR1);

    fclose(f_read);
    fclose(f_write);

    while (wait(NULL) > 0) {
        //wait
    }

    printf("Done\n");
    fflush(stdout);

    return 0;
}


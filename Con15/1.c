#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>

int cur;
pid_t pid, pid1, pid2;

void
handler(int signal)
{
    cur = signal;

    if (pid2 != 0) {
        kill(pid2, SIGALRM);
    }
}

void
first_child(void)
{
    sigset_t sig_1, sig_2;
    sigemptyset(&sig_1);
    sigaddset(&sig_1, SIGUSR1);
    sigaddset(&sig_1, SIGUSR2);
    sigaddset(&sig_1, SIGALRM);
    sigprocmask(SIG_BLOCK, &sig_1, &sig_2);

    int count_bits = 0;
    char buf = 0;

    while (1) {
        sigsuspend(&sig_2);

        if (cur == SIGUSR1) {
            buf <<= 1;
            count_bits++;
        } else if (cur == SIGUSR2) {
            buf = (buf << 1) | 1;
            count_bits++;
        } else if (cur == SIGIO) {
            _exit(0);
        }

        if (count_bits == 8) {
            putchar(buf);
            fflush(stdout);
            count_bits = 0;
            buf = 0;
        }

        kill(pid, SIGALRM);
    }
}

void
second_child(char *file)
{
    sigset_t sig_1, sig_2;

    sigemptyset(&sig_1);
    sigaddset(&sig_1, SIGALRM);
    sigprocmask(SIG_BLOCK, &sig_1, &sig_2);

    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        return 1;
    }

    char buf;
    while (read(fd, &buf, sizeof(buf)) == sizeof(buf)) {
        for (int i = CHAR_BIT - 1; i >= 0; i--) {
            if (buf & (1 << i)) {
                kill(pid1, SIGUSR2);
            } else {
                kill(pid1, SIGUSR1);
            }
            sigsuspend(&sig_2);
            cur = 0;
        }
    }

    if (close(fd) < 0) {
        return 1;
    }

    kill(pid1, SIGIO);
    _exit(0);
}

int
main(int argc, char *argv[])
{
    struct sigaction sig = {.sa_handler = handler, .sa_flags = SA_RESTART};
    sigaction(SIGUSR1, &sig, NULL);
    sigaction(SIGUSR2, &sig, NULL);
    sigaction(SIGIO, &sig, NULL);
    sigaction(SIGALRM, &sig, NULL);

    pid = getpid();

    if ((pid1 = fork()) < 0) {
        return 1;
    } else if (pid1 == 0) {
        first_child();
    }

    if ((pid2 = fork()) < 0) {
        return 1;
    } else if (pid2 == 0) {
        second_child(argv[1]);
    }

    while (wait(NULL) > 0);

    return 0;
}


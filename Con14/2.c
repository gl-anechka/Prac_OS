#include <stdio.h>
#include <signal.h>
#include <unistd.h>

enum
{
    MOD1,
    MOD2,
    SIG1 = SIGINT,
    SIG2 = SIGQUIT
};

volatile sig_atomic_t mode = MOD1;

void
handler(int sig)
{
    if (sig == SIG1) {
        mode = MOD1;
    }
    if (sig == SIG2) {
        mode = MOD2;
    }
}

int
main(void)
{
    sigaction(SIG1, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIG2, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    printf("%d\n", getpid());
    fflush(stdout);
    int res = 0;
    int value;

    while (scanf("%d", &value) == 1) {
        if (mode == MOD1) {
            __builtin_sadd_overflow(res, value, &res);
        } else if (mode == MOD2) {
            __builtin_smul_overflow(res, value, &res);
        }
        printf("%d\n", res);
        fflush(stdout);
    }

    return 0;
}
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

volatile sig_atomic_t count = 0;
volatile int prime = 0;

void
handler(int sig)
{
    if (sig == SIGINT) {
        if (count < 3) {
            printf("%d\n", prime);
            fflush(stdout);
            count++;
        } else {
            _exit(0);
        }
    }
    if (sig == SIGTERM) {
        _exit(0);
    }
}

int
is_prime(int n)
{
    if (n <= 1) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    double border = (int) (sqrt(n));
    for (int i = 2; i <= border + 1; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(void)
{
    sigaction(SIGINT, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGTERM, &(struct sigaction){.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    printf("%d\n", getpid());
    fflush(stdout);

    int low, high;
    scanf("%d%d", &low, &high);

    for (int i = low; i < high; i++) {
        if (is_prime(i)) {
            prime = i;
        }
    }

    signal(SIGINT, SIG_IGN);
    printf("-1\n");
    fflush(stdout);
    return 0;
}
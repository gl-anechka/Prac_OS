#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

enum
{
    MAX_COUNT = 5
};

volatile sig_atomic_t count = 0;

void
handler(int sig)
{
    signal(SIGHUP, handler);

    if (count < MAX_COUNT) {
        printf("%d\n", count++);
        fflush(stdout);
    } else {
        exit(0);
    }
}

int
main(void)
{
    signal(SIGHUP, handler);

    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        pause();
    }
    return 0;
}
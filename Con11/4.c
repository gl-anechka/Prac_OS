#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int
main(void)
{
    int n;
    scanf("%d", &n);

    pid_t pid;
    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
            printf("%d\n", i + 1);
            break;
        } else {
            printf("%d ", i + 1);
            fflush(stdout);
        }

        if ((pid = fork()) > 0) {
            wait(NULL);
            break;
        } else if (pid < 0) {
            return 1;
        }
    }

    return 0;
}
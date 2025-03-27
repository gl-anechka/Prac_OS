#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int
main(void)
{
    pid_t pid;
    if ((pid = fork()) > 0) {
        wait(NULL);
        printf("1\n");
    } else if ((pid = fork()) > 0) {
        wait(NULL);
        printf("2 ");
    } else {
        printf("3 ");
    }
    return 0;
}
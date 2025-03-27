#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

int
ifstatus(const char *name)
{
    int status;
    pid_t pid;

    if ((pid = fork()) < 0) {
        return 0;
    }

    if (pid == 0) {
        if (execlp(name, name, NULL) < 0) {
            return 0;
        }
    }
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && !WEXITSTATUS(status);
}

int
main(int argc, char *argv[])
{
    if (argc != 4) {
        return 1;
    }

    int res = (ifstatus(argv[1]) || ifstatus(argv[2])) && ifstatus(argv[3]);
    return !res;
}
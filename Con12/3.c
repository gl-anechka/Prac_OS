#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int
main(int argc, char *argv[])
{
    int count = 0;
    int status;

    for (int i = 1; i < argc; i++) {
        //параллельно
        if (argv[i][0] == 'p') {
            pid_t pid = fork();
            if (pid < 0) {
                return 1;
            }
            if (pid == 0) {
                if (execlp(argv[i] + 1, argv[i] + 1, NULL) < 0) {
                    _exit(1);
                }
            }
        }

        //последовательно
        if (argv[i][0] == 's') {
            while (wait(&status) > 0) {
                count += (WIFEXITED(status) && !WEXITSTATUS(status));
            }
            pid_t pid2 = fork();
            if (pid2 < 0) {
                return 1;
            }
            if (pid2 == 0) {
                if (execlp(argv[i] + 1, argv[i] + 1, NULL) < 0) {
                    _exit(1);
                }
            }
        }
    }

    while (wait(&status) > 0) {
        count += (WIFEXITED(status) && !WEXITSTATUS(status));
    }
    printf("%d\n", count);
    return 0;
}
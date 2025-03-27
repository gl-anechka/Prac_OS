#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

enum
{
    LEN_MAX = 9,
    BASIS = 10
};

int
main(void)
{
    pid_t pid;
    setbuf(stdin, NULL);

    for (int i = 0; i < 3; i++) {
        if ((pid = fork()) > 0) {
            char st[LEN_MAX];
            if (fgets(st, LEN_MAX, stdin) == NULL) {
                return 1;
            }

            long value = strtol(st, NULL, BASIS);

            //char *eptr = NULL;
            //errno = 0;
            //long value = strtol(st, &eptr, BASIS);
            //if (errno || *eptr || eptr == st || (int) value != value) {
            //    return 1;
            //}

            printf("%d %ld\n", i + 1, value * value);
            fflush(stdout);
            break;
        }
    }

    int status;
    while (wait(&status) > 0) {
        // ожидание завершения всех процессов
    }
    return 0;
}
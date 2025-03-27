#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/random.h>

const char script1[] = "#!/usr/bin/python3\n"
                       "import os\n"
                       "print(1";
const char script2[] = ")\n"
                       "os.remove(__file__)";
const char mult[] = " * ";

enum
{
    LEN_MUL = sizeof(mult) - 1,
    LEN_SCR_1 = sizeof(script1) - 1,
    LEN_SCR_2 = sizeof(script2) - 1,
    BUF_SIZE = 100
};

const char *
temp_path(void)
{
    char *temp = getenv("XDG_RUNTIME_DIR");
    if (temp != NULL) {
        return temp;
    }

    temp = getenv("TMPDIR");
    if (temp != NULL) {
        return temp;
    }

    return "/tmp";
}

int
main(int argc, char *argv[])
{
    const char *temp = temp_path();
    char path[PATH_MAX + BUF_SIZE] = {0};

    // создание уникального имени для файла
    char buf[BUF_SIZE] = {0};
    getrandom(buf, sizeof(buf), 0);
    snprintf(path, sizeof(path), "%s/abp_%s.py", temp, buf);

    int file = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0770);
    if (file < 0) {
        return 1;
    }

    if (write(file, script1, LEN_SCR_1) != LEN_SCR_1) {
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (write(file, mult, LEN_MUL) != LEN_MUL) {
            return 1;
        }

        int len = strlen(argv[i]);
        if (write(file, argv[i], len) != len) {
            return 1;
        }
    }

    if (write(file, script2, LEN_SCR_2) != LEN_SCR_2) {
        return 1;
    }

    if (close(file) < 0) {
        return 1;
    }

    if (execlp(path, path, NULL) < 0) {
        return 1;
    }

    return 0;
}
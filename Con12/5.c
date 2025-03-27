#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include <wait.h>

const char prog1[] = "#include <stdio.h>\n"
                     "#include <stdlib.h>\n"
                     "\n"
                     "enum\n"
                     "{\n"
                     "MAX_LEN = 15,\n"
                     "BASE = 10\n"
                     "};\n"
                     "\n"
                     "const char reject[] = \"reject\";\n"
                     "const char disqualify[] = \"disqualify\";\n"
                     "const char summon[] = \"summon\";\n"
                     "\n"
                     "int\n"
                     "main(void) {\n"
                     "char num[MAX_LEN];\n"
                     "while (scanf(\"%s\", num) == 1) {\n"
                     "int x = strtol(num, NULL, BASE);\n"
                     "printf(\"%s\\n\", ";
const char prog2[] = ");\n"
                     "}\n"
                     "return 0;\n"
                     "}\n";

enum
{
    BUF_SIZE = 100,
    RANDOM_BYTES = 16,
    BUF_LEN = 33,
    TYPE_LEN = 2
};

int
gen_random_suffix(char *buffer, int buffer_size)
{
    if (buffer_size < BUF_LEN) {
        return -1;
    }

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("cannot open /dev/urandom");
        return -1;
    }

    unsigned char random_bytes[RANDOM_BYTES];
    if (read(fd, random_bytes, RANDOM_BYTES) != RANDOM_BYTES) {
        perror("cannot read from /dev/urandom");
        close(fd);
        return -1;
    }
    close(fd);

    for (int i = 0; i < RANDOM_BYTES; ++i) {
        if (snprintf(&buffer[i * TYPE_LEN], TYPE_LEN + 1, "%02x", random_bytes[i]) >= TYPE_LEN + 1) {
            fprintf(stderr, "snprintf exceeded buffer bounds\n");
            return -1;
        }
    }

    buffer[BUF_LEN - 1] = '\0';
    return 0;
}

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

    char random_suffix[BUF_LEN];
    if (gen_random_suffix(random_suffix, sizeof(random_suffix)) != 0) {
        fprintf(stderr, "error generating random suffix\n");
        return 1;
    }

    char path1[PATH_MAX] = {0};
    char path2[PATH_MAX] = {0};

    // создание уникального имени для c-файла
    snprintf(path1, sizeof(path1), "%s/%s_prog.c", temp, random_suffix);

    // создание уникального имени для o-файла
    snprintf(path2, sizeof(path2), "%s/%s_prog.o", temp, random_suffix);

    /*c-файл*/
    int file = open(path1, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (file < 0) {
        fprintf(stderr, "error with file creation\n");
        return 1;
    }
    FILE *file_c = fdopen(file, "w");

    if (fprintf(file_c, "%s", prog1) < 0) {
        fprintf(stderr, "cannot write to file %s\n", path1);
        return 1;
    }
    if (fprintf(file_c, "%s", argv[1]) < 0) {
        fprintf(stderr, "cannot write to file %s\n", path1);
        return 1;
    }
    if (fprintf(file_c, "%s", prog2) < 0) {
        fprintf(stderr, "cannot write to file %s\n", path1);
        return 1;
    }

    fclose(file_c);

    int status;
    pid_t pid;
    if ((pid = fork()) < 0) {
        fprintf(stderr, "process has not been created");
        return 1;
    } else if (pid == 0) {
        if (execlp("gcc", "gcc", path1, "-o", path2, NULL) < 0) {
            fprintf(stderr, "error in function \"execlp\"");
            _exit(1);
        }
        _exit(0);
    } else {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            fprintf(stderr, "error in function \"execlp\"");
            return 1;
        }

        if (execlp(path2, path2, NULL) < 0) {
            fprintf(stderr, "error in function \"execlp\"");
            _exit(1);
        }
    }

    if (remove(path1) < 0) {
        fprintf(stderr, "cannot remove file %s", path1);
        return 1;
    }
    if (remove(path2) < 0) {
        fprintf(stderr, "cannot remove file %s", path2);
        return 1;
    }

    return 0;
}

#include <stdio.h>

const char parent[] = "/..";
const char curent[] = "/.";

enum
{
    DOT = '.',
    SLASH = '/',
    END_STR = '\0',
    LEN_PAR = sizeof(parent) - 1,
    LEN_CUR = sizeof(curent) - 1
};

void
normalize_path(char *buf)
{
    char *cur = buf; // исходная строка
    char *res = buf; // результат

    while (cur[0] != END_STR) {
        // пропустить все '.'
        if (cur[0] == DOT && (cur[1] == SLASH || cur[1] == END_STR) && cur[-1] == SLASH) {
            if (cur[1] == END_STR) {
                cur += LEN_CUR - 1;
            } else {
                cur += LEN_CUR;
            }
            continue;
        }

        // обработка '..'
        if (cur[0] == DOT && cur[1] == DOT && (cur[2] == SLASH || cur[2] == END_STR) && cur[-1] == SLASH) {
            if (res > buf + 1) {
                res--;
            }
            while (res > buf && res[-1] != SLASH) {
                res--;
            }
            if (cur[2] == END_STR) {
                cur += LEN_PAR - 1;
            } else {
                cur += LEN_PAR;
            }
            continue;
        }

        *res++ = *cur++;
    }

    // удалить последний '/'
    if (res != buf + 1 && res[-1] == SLASH) {
        res--;
    }

    *res = END_STR;
}

int main() {
    char path[] = "/a/b/../../a/./b/";
    normalize_path(path);
    printf("%s\n", path);
    return 0;
}
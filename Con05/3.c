#include <stdio.h>
#include <string.h>

const char rights[] = "rwxrwxrwx";

enum
{
    LEN_CONST = sizeof(rights) - 1 // длина константной строки
};

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    int len = strlen(str); // длина исходной строки
    if (len != LEN_CONST) {
        return -1;
    }

    unsigned int res = 0;
    for (int i = 0; i < len; i++) {
        res = res << 1;

        if (str[i] == rights[i]) {
            res = res | 1;
        } else if (str[i] != '-') {
            return -1;
        }
    }
    return res;
}

int
main()
{
    char arr[20];
    for(int i = 0; i < 5; i++) {
        scanf("%s", arr);
        int a = parse_rwx_permissions(&arr);
        printf("res = %d\n", a);
    }
    return 0;
}
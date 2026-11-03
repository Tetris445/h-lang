#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    char *cmd = argv[1];

    // sqrt (квадратный корень)
    if (strncmp(cmd, "sqrt ", 5) == 0) {
        double x = atof(cmd + 5);
        printf("%f\n", sqrt(x));
    }
    // len (длина строки)
    else if (strncmp(cmd, "len ", 4) == 0) {
        printf("%lu\n", strlen(cmd + 4));
    }
    // upper (верхний регистр) - упрощенно
    else if (strncmp(cmd, "upper ", 6) == 0) {
        char *s = cmd + 6;
        for (int i = 0; s[i]; i++) {
            if (s[i] >= 'a' && s[i] <= 'z') s[i] -= 32;
        }
        printf("%s\n", s);
    }
    return 0;
}

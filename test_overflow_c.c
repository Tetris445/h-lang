#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));
    char buf[256];

    printf("Генерируем огромную строку...\n");
    int i = 0;
    int long_str = "";
    while (i < 10000) {
    int long_str = long_str + A;
    i = i + 1;
    }
    printf("Отправляем в C (без проверки):\n");
    system("~/C-lang/c_echo ");
    printf("%d\n", long_str);
    free(pool);
    return 0;
}

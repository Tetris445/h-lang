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

    printf("Введи число:\n");
    int num = input();
    int res = num + 10;
    printf("Результат:\n");
    printf("res\n");
    free(pool);
    return 0;
}

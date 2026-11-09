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

    printf("\033[31mЧистый красный\033[0m\n");
    printf("\033[34mЧистый синий\033[0m\n");
    printf("\033[35mЧистый пурпурный\033[0m\n");
    printf("\033[36mЧистый голубой\033[0m\n");
    printf("\033[37mЧистый белый\033[0m\n");
    printf("\033[30mЧистый чёрный\033[0m\n");
    printf("[35mКрасный + Синий = Пурпурный\033[0m\n");
    printf("[37mЖёлтый + Синий = Зелёный\033[0m\n");
    printf("[33mКрасный + Зелёный = Жёлтый\033[0m\n");
    printf("[92mЗелёный + Белый = Салатовый\033[0m\n");
    free(pool);
    return 0;
}

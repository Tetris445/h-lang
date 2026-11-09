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

    printf("=========================================\n");
    printf("   СИСТЕМНЫЙ МОНИТОР (h-lang)\n");
    printf("=========================================\n");
    printf("--- Батарея ---\n");
    system("termux-battery-status | grep -E '"percentage"|"temperature"'");
    printf("--- Диск ---\n");
    system("df -h /data | tail -n 1");
    printf("--- Время ---\n");
    { time_t t = time(NULL); struct tm *tm = localtime(&t); printf("\033[33m%02d:%02d:%02d\033[0m\n", tm->tm_hour, tm->tm_min, tm->tm_sec); }
    printf("--- Память ---\n");
    pool = malloc(1 * 1024 * 1024);
    printf("Пул памяти создан: 1 МБ\n");
    int *report = (int*)((char*)pool + used);
    used += 1 * sizeof(int);
    printf("Контейнер 'report' создан\n");
    report[0] = 100;
    printf("Память инициализирована (1 МБ)\n");
    printf("=========================================\n");
    printf("Мониторинг завершен!\n");
    free(pool);
    return 0;
}

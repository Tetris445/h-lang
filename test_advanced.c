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

    printf("Математика:\n");
    printf("%f\n", sin(0));
    printf("%f\n", cos(0));
    printf("%f\n", pow(2, 3));
    printf("Файлы:\n");
    { FILE *f = fopen("test.txt", "w"); if (f) { fprintf(f, "%s", ""Привет из файла!""); fclose(f); printf("Записано в test.txt\n"); } }
    { FILE *f = fopen("test.txt", "r"); if (f) { while (fgets(buf, sizeof(buf), f)) printf("%s", buf); fclose(f); } }
    printf("Память:\n");
    pool = malloc(10 * 1024 * 1024);
    printf("Пул памяти создан: 10 МБ\n");
    int *arr = (int*)((char*)pool + used);
    used += 3 * sizeof(int);
    printf("Контейнер 'arr' создан\n");
    arr[0] = 100;
    { FILE *f = fopen("data.bin", "wb"); if (f) { fwrite(pool, 1, used, f); fclose(f); printf("Память сохранена в data.bin\n"); } }
    { FILE *f = fopen("data.bin", "rb"); if (f) { fread(pool, 1, used, f); fclose(f); printf("Память загружена из data.bin\n"); } }
    printf("%d\n", arr[0]);
    free(pool);
    return 0;
}

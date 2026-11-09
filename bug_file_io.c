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

    printf("1. Записываем данные в файл через file_write:\n");
    { FILE *f = fopen(""test.txt"", "w"); if (f) { fprintf(f, "%s", "\"Привет, h!\""); fclose(f); printf("Записано в "test.txt"\n"); } }
    printf("2. Читаем данные через file_read:\n");
    { FILE *f = fopen(""test.txt"", "r"); if (f) { while (fgets(buf, sizeof(buf), f)) printf("%s", buf); fclose(f); } }
    free(pool);
    return 0;
}

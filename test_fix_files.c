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
    system("echo "Привет, это фикс!" > "test_fixed.txt"");
    printf("2. Читаем данные через file_read:\n");
    system("cat "test_fixed.txt"");
    printf("3. Проверяем через sys:\n");
    system("cat test_fixed.txt");
    free(pool);
    return 0;
}

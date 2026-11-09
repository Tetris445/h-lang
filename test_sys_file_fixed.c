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

    printf("1. Создаем пустой файл:\n");
    system("touch compiled_data.txt");
    printf("2. Записываем данные в файл через sys:\n");
    system("echo "777" > compiled_data.txt");
    printf("3. Читаем данные через sys:\n");
    system("cat compiled_data.txt");
    printf("4. Проверяем размер:\n");
    system("ls -la compiled_data.txt");
    free(pool);
    return 0;
}

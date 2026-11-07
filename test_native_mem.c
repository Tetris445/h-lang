// Сгенерировано hcompiler из test_native_mem.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Память (заглушка)
    void *pool = NULL;
    int used = 0;

    printf("Создаем пул памяти:\n");
    // mem_init 10 (заглушка)
    pool = malloc(10 * 1024 * 1024);
    printf("Пул памяти создан: 10 МБ\n");
    printf("Создаем контейнер hp:\n");
    // mem_alloc hp 1 (заглушка)
    int *hp = (int*)((char*)pool + used);
    used += 1 * sizeof(int);
    printf("Контейнер 'hp' создан\n");
    printf("Записываем 100:\n");
    hp[0] = 100;
    printf("Читаем:\n");
    printf("%d\n", hp[0]);
    printf("Занято: %d байт\n", used);
    free(pool);
    return 0;
}

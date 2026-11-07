// Сгенерировано hcompiler из test_full_native.h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));

    int x = 0;
    while (x < 3) {
    printf("Привет\n");
    x = x + 1;
    }
    printf("%d\n", 1 + rand() % (10 - 1 + 1));
    free(pool);
    return 0;
}

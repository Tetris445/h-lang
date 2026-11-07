#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));
    char buf[256];

    printf("Проверка sys:\n");
    system("echo "Привет из sys!"");
    printf("Проверка net:\n");
    system("curl -s https://api.github.com");
    free(pool);
    return 0;
}

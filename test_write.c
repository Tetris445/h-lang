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

    printf("Пишем данные:\n");
    system("echo "777" > my_data.txt");
    printf("Читаем:\n");
    system("cat my_data.txt");
    free(pool);
    return 0;
}

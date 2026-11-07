// Сгенерировано hcompiler из calc.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));
    char buf[256];

    printf("Калькулятор на h\n");
    printf("Введи первое число:\n");
    fgets(buf, sizeof(buf), stdin);
    int a = atoi(buf);
    printf("Введи второе число:\n");
    fgets(buf, sizeof(buf), stdin);
    int b = atoi(buf);
    printf("Выбери операцию (add, sub, mul, div):\n");
    int op = input();
    if (op == add) {
    printf("%d\n", a + b);
    }
    if (op == sub) {
    printf("%d\n", a - b);
    }
    if (op == mul) {
    printf("%d\n", a * b);
    }
    if (op == div) {
    if (b != 0) printf("%d\n", a / b);
    }
    free(pool);
    return 0;
}

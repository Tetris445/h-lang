#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MEM 100

char mem[MAX_MEM][256];

void print_error(const char *color, const char *type, const char *msg) {
    printf("\033[%sm[%s] %s\033[0m\n", color, type, msg);
}

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: hvm_asm_bridge <file.h>\n"); return 1; }
    FILE *file = fopen(argv[1], "r");
    if (!file) { print_error("35", "FATAL", "File not found"); return 1; }

    srand(time(NULL));
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (p[0] == '\0' || p[0] == '#') continue;

        // === A0: Очистка памяти ===
        if (strncmp(p, "A0", 2) == 0) {
            for (int i = 0; i < MAX_MEM; i++) mem[i][0] = '\0';
            printf("\033[32m[OK] A0: Память очищена\033[0m\n");
        }
        // === A1: Запись в mem[1] ===
        else if (strncmp(p, "A1 ", 3) == 0) {
            char *text = p + 3;
            if (text[0] == '"' || text[0] == '\'') text++;
            int len = strlen(text);
            if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
            strcpy(mem[1], text);
            printf("\033[32m[OK] A1: Записано '%s' в mem[1]\033[0m\n", text);
        }
        // === A2: Запись в mem[2] ===
        else if (strncmp(p, "A2 ", 3) == 0) {
            char *text = p + 3;
            if (text[0] == '"' || text[0] == '\'') text++;
            int len = strlen(text);
            if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
            strcpy(mem[2], text);
            printf("\033[32m[OK] A2: Записано '%s' в mem[2]\033[0m\n", text);
        }
        // === A3: Сложение ===
        else if (strncmp(p, "A3", 2) == 0) {
            if (mem[1][0] == '\0' || mem[2][0] == '\0') {
                print_error("34", "WARN", "A3: Одна из ячеек пуста (mem[1] или mem[2])");
                continue;
            }
            int a = atoi(mem[1]), b = atoi(mem[2]);
            sprintf(mem[3], "%d", a + b);
            printf("\033[32m[OK] A3: %d + %d = %s (mem[3])\033[0m\n", a, b, mem[3]);
        }
        // === A4: Вычитание ===
        else if (strncmp(p, "A4", 2) == 0) {
            if (mem[1][0] == '\0' || mem[2][0] == '\0') {
                print_error("34", "WARN", "A4: Одна из ячеек пуста (mem[1] или mem[2])");
                continue;
            }
            int a = atoi(mem[1]), b = atoi(mem[2]);
            sprintf(mem[4], "%d", a - b);
            printf("\033[32m[OK] A4: %d - %d = %s (mem[4])\033[0m\n", a, b, mem[4]);
        }
        // === A5: Умножение ===
        else if (strncmp(p, "A5", 2) == 0) {
            if (mem[1][0] == '\0' || mem[2][0] == '\0') {
                print_error("34", "WARN", "A5: Одна из ячеек пуста (mem[1] или mem[2])");
                continue;
            }
            int a = atoi(mem[1]), b = atoi(mem[2]);
            sprintf(mem[5], "%d", a * b);
            printf("\033[32m[OK] A5: %d * %d = %s (mem[5])\033[0m\n", a, b, mem[5]);
        }
        // === A6: Деление ===
        else if (strncmp(p, "A6", 2) == 0) {
            if (mem[1][0] == '\0' || mem[2][0] == '\0') {
                print_error("34", "WARN", "A6: Одна из ячеек пуста (mem[1] или mem[2])");
                continue;
            }
            int a = atoi(mem[1]), b = atoi(mem[2]);
            if (b == 0) print_error("35", "FATAL", "A6: Деление на ноль!");
            else { sprintf(mem[6], "%d", a / b); printf("\033[32m[OK] A6: %d / %d = %s (mem[6])\033[0m\n", a, b, mem[6]); }
        }
        // === A7: Длина mem[1] ===
        else if (strncmp(p, "A7", 2) == 0) {
            if (mem[1][0] == '\0') {
                print_error("34", "WARN", "A7: mem[1] пуста (нечего считать)");
                continue;
            }
            sprintf(mem[7], "%lu", strlen(mem[1]));
            printf("\033[32m[OK] A7: Длина '%s' = %s (mem[7])\033[0m\n", mem[1], mem[7]);
        }
        // === A8: Верхний регистр ===
        else if (strncmp(p, "A8", 2) == 0) {
            if (mem[1][0] == '\0') {
                print_error("34", "WARN", "A8: mem[1] пуста (нечего преобразовывать)");
                continue;
            }
            strcpy(mem[8], mem[1]);
            for (int i = 0; mem[8][i]; i++) mem[8][i] = toupper(mem[8][i]);
            printf("\033[32m[OK] A8: Верхний регистр: %s (mem[8])\033[0m\n", mem[8]);
        }
        // === A9: Случайное число ===
        else if (strncmp(p, "A9", 2) == 0) {
            sprintf(mem[9], "%d", rand() % 100 + 1);
            printf("\033[32m[OK] A9: Случайное число: %s (mem[9])\033[0m\n", mem[9]);
        }
        // === A10: Вывод mem[1] ===
        else if (strncmp(p, "A10", 3) == 0) {
            if (mem[1][0] == '\0') {
                print_error("34", "WARN", "A10: mem[1] пуста (нечего выводить)");
                continue;
            }
            printf("\033[33m[RESULT] A10: %s\033[0m\n", mem[1]);
        }
        // === B1: Запись в файл ===
        else if (strncmp(p, "B1", 2) == 0) {
            if (mem[1][0] == '\0' || mem[2][0] == '\0') {
                print_error("34", "WARN", "B1: mem[1] или mem[2] пуста (нечего записывать)");
                continue;
            }
            FILE *f = fopen(mem[2], "w");
            if (f) { fprintf(f, "%s", mem[1]); fclose(f); printf("\033[32m[OK] B1: Записано в '%s'\033[0m\n", mem[2]); }
            else print_error("31", "ERR", "B1: Не удалось открыть файл");
        }
        // === B2: Чтение файла ===
        else if (strncmp(p, "B2", 2) == 0) {
            if (mem[1][0] == '\0') {
                print_error("34", "WARN", "B2: mem[1] пуста (нечего читать)");
                continue;
            }
            FILE *f = fopen(mem[1], "r");
            if (f) { fgets(mem[10], sizeof(mem[10]), f); fclose(f); printf("\033[32m[OK] B2: Прочитано: %s (mem[10])\033[0m\n", mem[10]); }
            else print_error("31", "ERR", "B2: Файл не найден");
        }
        // === B3: Вывод mem[10] ===
        else if (strncmp(p, "B3", 2) == 0) {
            if (mem[10][0] == '\0') {
                print_error("34", "WARN", "B3: mem[10] пуста (нечего выводить)");
                continue;
            }
            printf("\033[33m[RESULT] B3: %s\033[0m\n", mem[10]);
        }
        // === Неизвестная команда ===
        else {
            print_error("31", "ERR", "Неизвестная команда (A0-A10, B1-B3)");
        }
    }

    fclose(file);
    return 0;
}

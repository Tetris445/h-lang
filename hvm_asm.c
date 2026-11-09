#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CELLS 10

char memory[MAX_CELLS][256]; // Ячейки памяти A0-A9
int cell_count = 0;

void print_error(const char *color, const char *type, const char *msg) {
    printf("\033[%sm[%s ERROR] %s\033[0m\n", color, type, msg);
}

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: hvm_asm <file.h>\n"); return 1; }
    FILE *file = fopen(argv[1], "r");
    if (!file) { printf("\033[31mFile not found\033[0m\n"); return 1; }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (p[0] == '\0' || p[0] == '#') continue;

        // A0: Инициализация (очистка памяти)
        if (strncmp(p, "A0", 2) == 0) {
            for (int i = 0; i < MAX_CELLS; i++) memory[i][0] = '\0';
            cell_count = 0;
            printf("\033[32m[OK] A0: Память очищена\033[0m\n");
        }
        // A1: Запись в ячейку 1
        else if (strncmp(p, "A1 ", 3) == 0) {
            char *text = p + 3;
            if (text[0] == '"' || text[0] == '\'') text++;
            int len = strlen(text);
            if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
            
            if (strlen(text) >= 256) {
                print_error("35", "FATAL", "A1: Переполнение памяти (ячейка 1)");
                return 1;
            }
            strcpy(memory[1], text);
            printf("\033[32m[OK] A1: Записано '%s' в ячейку 1\033[0m\n", text);
        }
        // A2: Копирование из A1 в A2
        else if (strncmp(p, "A2", 2) == 0) {
            if (memory[1][0] == '\0') {
                print_error("34", "WARN", "A2: Ячейка 1 пуста (нечего копировать)");
            } else {
                strcpy(memory[2], memory[1]);
                printf("\033[32m[OK] A2: Скопировано из ячейки 1 в 2\033[0m\n");
            }
        }
        // A3: Копирование из A2 в A3
        else if (strncmp(p, "A3", 2) == 0) {
            if (memory[2][0] == '\0') {
                print_error("34", "WARN", "A3: Ячейка 2 пуста (нечего копировать)");
            } else {
                strcpy(memory[3], memory[2]);
                printf("\033[32m[OK] A3: Скопировано из ячейки 2 в 3\033[0m\n");
            }
        }
        // A4: Копирование из A3 в A4
        else if (strncmp(p, "A4", 2) == 0) {
            if (memory[3][0] == '\0') {
                print_error("34", "WARN", "A4: Ячейка 3 пуста (нечего копировать)");
            } else {
                strcpy(memory[4], memory[3]);
                printf("\033[32m[OK] A4: Скопировано из ячейки 3 в 4\033[0m\n");
            }
        }
        // A5: Вывод содержимого A4
        else if (strncmp(p, "A5", 2) == 0) {
            if (memory[4][0] == '\0') {
                print_error("31", "ERR", "A5: Ячейка 4 пуста (нечего выводить)");
            } else {
                printf("\033[33m[RESULT] A5: %s\033[0m\n", memory[4]);
            }
        }
        // Неизвестная команда
        else {
            print_error("31", "ERR", "Неизвестная команда (только A0-A5)");
        }
    }

    fclose(file);
    return 0;
}

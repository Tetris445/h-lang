#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: h2c <file.h>\n"); return 1; }
    FILE *file = fopen(argv[1], "r");
    if (!file) { printf("File not found\n"); return 1; }

    char line[1024];
    int vars[100];
    char var_names[100][50];
    int var_count = 0;
    int skip_to_end = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;

        if (skip_to_end) {
            if (strncmp(line, "end", 3) == 0) skip_to_end = 0;
            continue;
        }

        // IF
        if (strncmp(line, "if ", 3) == 0) {
            char a[50], op[5], b[50];
            if (sscanf(line + 3, "%s %s %s", a, op, b) == 3) {
                int val_a = atoi(a), val_b = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) val_a = vars[i];
                    if (strcmp(var_names[i], b) == 0) val_b = vars[i];
                }
                int result = 0;
                if (strcmp(op, ">") == 0) result = val_a > val_b;
                else if (strcmp(op, "<") == 0) result = val_a < val_b;
                else if (strcmp(op, "==") == 0) result = val_a == val_b;
                if (!result) skip_to_end = 1;
            }
            continue;
        }
        if (strncmp(line, "end", 3) == 0) continue;

        // ВЫВОД
        if (strncmp(line, "say ", 4) == 0) {
            printf("%s\n", line + 4);
        }
        // ЦВЕТА
        else if (strncmp(line, "red ", 4) == 0) { printf("\x1b[31m%s\x1b[0m\n", line + 4); }
        else if (strncmp(line, "green ", 6) == 0) { printf("\x1b[32m%s\x1b[0m\n", line + 6); }
        else if (strncmp(line, "yellow ", 7) == 0) { printf("\x1b[33m%s\x1b[0m\n", line + 7); }
        else if (strncmp(line, "blue ", 5) == 0) { printf("\x1b[34m%s\x1b[0m\n", line + 5); }
        // ВРЕМЯ
        else if (strncmp(line, "now", 3) == 0) {
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            printf("%02d/%02d/%04d, %02d:%02d:%02d\n", tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);
        }
        else if (strncmp(line, "year", 4) == 0) { time_t t = time(NULL); struct tm *tm = localtime(&t); printf("%d\n", tm->tm_year + 1900); }
        else if (strncmp(line, "month", 5) == 0) { time_t t = time(NULL); struct tm *tm = localtime(&t); printf("%d\n", tm->tm_mon + 1); }
        else if (strncmp(line, "day", 3) == 0) { time_t t = time(NULL); struct tm *tm = localtime(&t); printf("%d\n", tm->tm_mday); }
        // ПЕРЕМЕННЫЕ
        else if (strncmp(line, "let ", 4) == 0) {
            char name[50]; int value;
            if (sscanf(line + 4, "%s = %d", name, &value) == 2) {
                strcpy(var_names[var_count], name);
                vars[var_count] = value;
                var_count++;
            }
        }
        // МАТЕМАТИКА
        else if (strncmp(line, "add ", 4) == 0) { char a[50], b[50]; if (sscanf(line + 4, "%s %s", a, b) == 2) { int va = atoi(a), vb = atoi(b); for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], a) == 0) va = vars[i]; if (strcmp(var_names[i], b) == 0) vb = vars[i]; } printf("%d\n", va + vb); } }
        else if (strncmp(line, "sub ", 4) == 0) { char a[50], b[50]; if (sscanf(line + 4, "%s %s", a, b) == 2) { int va = atoi(a), vb = atoi(b); for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], a) == 0) va = vars[i]; if (strcmp(var_names[i], b) == 0) vb = vars[i]; } printf("%d\n", va - vb); } }
        else if (strncmp(line, "mul ", 4) == 0) { char a[50], b[50]; if (sscanf(line + 4, "%s %s", a, b) == 2) { int va = atoi(a), vb = atoi(b); for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], a) == 0) va = vars[i]; if (strcmp(var_names[i], b) == 0) vb = vars[i]; } printf("%d\n", va * vb); } }
        else if (strncmp(line, "div ", 4) == 0) { char a[50], b[50]; if (sscanf(line + 4, "%s %s", a, b) == 2) { int va = atoi(a), vb = atoi(b); for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], a) == 0) va = vars[i]; if (strcmp(var_names[i], b) == 0) vb = vars[i]; } if (vb != 0) printf("%d\n", va / vb); } }
    }

    fclose(file);
    return 0;
}

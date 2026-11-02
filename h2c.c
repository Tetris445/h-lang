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
    int arrays[100][20];
    int array_sizes[100];
    char array_names[100][50];
    int array_count = 0;
    int skip_to_end = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;

        if (skip_to_end) {
            if (strncmp(line, "end", 3) == 0) skip_to_end = 0;
            continue;
        }

        // ВЛОЖЕННАЯ МАТЕМАТИКА
        if (strncmp(line, "mul add ", 8) == 0) {
            int a, b, c;
            if (sscanf(line + 8, "%d %d %d", &a, &b, &c) == 3) printf("%d\n", a * (b + c));
            continue;
        }
        if (strncmp(line, "add mul ", 8) == 0) {
            int a, b, c;
            if (sscanf(line + 8, "%d %d %d", &a, &b, &c) == 3) printf("%d\n", a + (b * c));
            continue;
        }

        // IF С AND / OR
        if (strncmp(line, "if ", 3) == 0) {
            char cond[1024];
            strcpy(cond, line + 3);
            int result = 0;

            // Проверка на AND
            char *and_pos = strstr(cond, " and ");
            if (and_pos != NULL) {
                *and_pos = '\0';
                char *cond1 = cond;
                char *cond2 = and_pos + 5;
                char a1[50], op1[5], b1[50], a2[50], op2[5], b2[50];
                int res1 = 0, res2 = 0;
                if (sscanf(cond1, "%s %s %s", a1, op1, b1) == 3) {
                    int v1 = atoi(a1), v2 = atoi(b1);
                    for (int i = 0; i < var_count; i++) {
                        if (strcmp(var_names[i], a1) == 0) v1 = vars[i];
                        if (strcmp(var_names[i], b1) == 0) v2 = vars[i];
                    }
                    if (strcmp(op1, ">") == 0) res1 = v1 > v2;
                    else if (strcmp(op1, "<") == 0) res1 = v1 < v2;
                    else if (strcmp(op1, "==") == 0) res1 = v1 == v2;
                }
                if (sscanf(cond2, "%s %s %s", a2, op2, b2) == 3) {
                    int v1 = atoi(a2), v2 = atoi(b2);
                    for (int i = 0; i < var_count; i++) {
                        if (strcmp(var_names[i], a2) == 0) v1 = vars[i];
                        if (strcmp(var_names[i], b2) == 0) v2 = vars[i];
                    }
                    if (strcmp(op2, ">") == 0) res2 = v1 > v2;
                    else if (strcmp(op2, "<") == 0) res2 = v1 < v2;
                    else if (strcmp(op2, "==") == 0) res2 = v1 == v2;
                }
                result = res1 && res2;
            }
            // Проверка на OR
            else {
                char *or_pos = strstr(cond, " or ");
                if (or_pos != NULL) {
                    *or_pos = '\0';
                    char *cond1 = cond;
                    char *cond2 = or_pos + 4;
                    char a1[50], op1[5], b1[50], a2[50], op2[5], b2[50];
                    int res1 = 0, res2 = 0;
                    if (sscanf(cond1, "%s %s %s", a1, op1, b1) == 3) {
                        int v1 = atoi(a1), v2 = atoi(b1);
                        for (int i = 0; i < var_count; i++) {
                            if (strcmp(var_names[i], a1) == 0) v1 = vars[i];
                            if (strcmp(var_names[i], b1) == 0) v2 = vars[i];
                        }
                        if (strcmp(op1, ">") == 0) res1 = v1 > v2;
                        else if (strcmp(op1, "<") == 0) res1 = v1 < v2;
                        else if (strcmp(op1, "==") == 0) res1 = v1 == v2;
                    }
                    if (sscanf(cond2, "%s %s %s", a2, op2, b2) == 3) {
                        int v1 = atoi(a2), v2 = atoi(b2);
                        for (int i = 0; i < var_count; i++) {
                            if (strcmp(var_names[i], a2) == 0) v1 = vars[i];
                            if (strcmp(var_names[i], b2) == 0) v2 = vars[i];
                        }
                        if (strcmp(op2, ">") == 0) res2 = v1 > v2;
                        else if (strcmp(op2, "<") == 0) res2 = v1 < v2;
                        else if (strcmp(op2, "==") == 0) res2 = v1 == v2;
                    }
                    result = res1 || res2;
                } else {
                    // Простое условие
                    char a[50], op[5], b[50];
                    if (sscanf(cond, "%s %s %s", a, op, b) == 3) {
                        int v1 = atoi(a), v2 = atoi(b);
                        for (int i = 0; i < var_count; i++) {
                            if (strcmp(var_names[i], a) == 0) v1 = vars[i];
                            if (strcmp(var_names[i], b) == 0) v2 = vars[i];
                        }
                        if (strcmp(op, ">") == 0) result = v1 > v2;
                        else if (strcmp(op, "<") == 0) result = v1 < v2;
                        else if (strcmp(op, "==") == 0) result = v1 == v2;
                    }
                }
            }
            if (!result) skip_to_end = 1;
            continue;
        }
        if (strncmp(line, "end", 3) == 0) continue;

        // SAY
        if (strncmp(line, "say ", 4) == 0) {
            char *arg = line + 4;
            int is_array = 0;
            for (int i = 0; i < array_count; i++) {
                if (strcmp(array_names[i], arg) == 0) {
                    printf("[ ");
                    for (int j = 0; j < array_sizes[i]; j++) printf("%d ", arrays[i][j]);
                    printf("]\n");
                    is_array = 1;
                    break;
                }
            }
            if (!is_array) printf("%s\n", arg);
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
        // МАССИВЫ
        else if (strncmp(line, "let ", 4) == 0 && strchr(line, '[') != NULL) {
            char name[50];
            int values[20], count = 0;
            if (sscanf(line + 4, "%s = [", name) == 1) {
                char *ptr = strchr(line, '[') + 1;
                while (*ptr != ']' && *ptr != '\0') {
                    if (*ptr >= '0' && *ptr <= '9') {
                        values[count++] = atoi(ptr);
                        while (*ptr >= '0' && *ptr <= '9') ptr++;
                    } else ptr++;
                }
                strcpy(array_names[array_count], name);
                for (int i = 0; i < count; i++) arrays[array_count][i] = values[i];
                array_sizes[array_count] = count;
                array_count++;
            }
        }
        // PUSH
        else if (strncmp(line, "push ", 5) == 0) {
            char name[50]; int value;
            if (sscanf(line + 5, "%s %d", name, &value) == 2) {
                for (int i = 0; i < array_count; i++) {
                    if (strcmp(array_names[i], name) == 0) { arrays[i][array_sizes[i]++] = value; break; }
                }
            }
        }
        // ПЕРЕМЕННЫЕ
        else if (strncmp(line, "let ", 4) == 0) {
            char name[50]; int value;
            if (sscanf(line + 4, "%s = %d", name, &value) == 2) {
                strcpy(var_names[var_count], name); vars[var_count] = value; var_count++;
            }
        }
        // МАТЕМАТИКА
        else if (strncmp(line, "add ", 4) == 0) {
            char a[50], b[50];
            if (sscanf(line + 4, "%s %s", a, b) == 2) {
                int val_a = atoi(a), val_b = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) val_a = vars[i];
                    if (strcmp(var_names[i], b) == 0) val_b = vars[i];
                }
                printf("%d\n", val_a + val_b);
            }
        }
        else if (strncmp(line, "mul ", 4) == 0) {
            char a[50], b[50];
            if (sscanf(line + 4, "%s %s", a, b) == 2) {
                int val_a = atoi(a), val_b = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) val_a = vars[i];
                    if (strcmp(var_names[i], b) == 0) val_b = vars[i];
                }
                printf("%d\n", val_a * val_b);
            }
        }
        else if (strncmp(line, "sub ", 4) == 0) {
            char a[50], b[50];
            if (sscanf(line + 4, "%s %s", a, b) == 2) {
                int val_a = atoi(a), val_b = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) val_a = vars[i];
                    if (strcmp(var_names[i], b) == 0) val_b = vars[i];
                }
                printf("%d\n", val_a - val_b);
            }
        }
        else if (strncmp(line, "div ", 4) == 0) {
            char a[50], b[50];
            if (sscanf(line + 4, "%s %s", a, b) == 2) {
                int val_a = atoi(a), val_b = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) val_a = vars[i];
                    if (strcmp(var_names[i], b) == 0) val_b = vars[i];
                }
                if (val_b != 0) printf("%d\n", val_a / val_b);
            }
        }
    }

    fclose(file);
    return 0;
}

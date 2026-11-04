#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_VARS 100

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: h2c <file.h>\n"); return 1; }
    FILE *file = fopen(argv[1], "r");
    if (!file) { printf("\033[31mFile not found\033[0m\n"); return 1; }

    char line[1024];
    int vars[MAX_VARS];
    char var_names[MAX_VARS][64];
    int var_count = 0;
    int skip_to_end = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;

        if (skip_to_end) {
            if (strncmp(line, "end", 3) == 0) skip_to_end = 0;
            continue;
        }

        if (strncmp(line, "version", 7) == 0) { printf("\033[33mh2c v2.4.5\033[0m\n"); continue; }

        // ВРЕМЯ
        if (strncmp(line, "now", 3) == 0) {
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            printf("\033[33m%02d:%02d:%02d\033[0m\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
            continue;
        }

        // ЦВЕТА
        if (strncmp(line, "red ", 4) == 0) {
            char *s = line + 4;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            printf("\033[31m%s\033[0m\n", s);
            continue;
        }
        if (strncmp(line, "green ", 6) == 0) {
            char *s = line + 6;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            printf("\033[32m%s\033[0m\n", s);
            continue;
        }
        if (strncmp(line, "yellow ", 7) == 0) {
            char *s = line + 7;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            printf("\033[33m%s\033[0m\n", s);
            continue;
        }
        if (strncmp(line, "blue ", 5) == 0) {
            char *s = line + 5;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            printf("\033[34m%s\033[0m\n", s);
            continue;
        }

        // IF
        if (strncmp(line, "if ", 3) == 0) {
            char a[50], op[5], b[50];
            if (sscanf(line + 3, "%s %s %s", a, op, b) == 3) {
                int va = atoi(a), vb = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) va = vars[i];
                    if (strcmp(var_names[i], b) == 0) vb = vars[i];
                }
                int result = 0;
                if (strcmp(op, ">") == 0) result = va > vb;
                else if (strcmp(op, "<") == 0) result = va < vb;
                else if (strcmp(op, "==") == 0) result = va == vb;
                if (!result) skip_to_end = 1;
            } else {
                printf("\033[31mError: Invalid IF syntax\033[0m\n");
            }
            continue;
        }

        // LET
        if (strncmp(line, "let ", 4) == 0) {
            char name[64], eq[5], val[64];
            if (sscanf(line + 4, "%s %s %s", name, eq, val) == 3) {
                int v = atoi(val);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], val) == 0) v = vars[i];
                }
                int found = 0;
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], name) == 0) { vars[i] = v; found = 1; break; }
                }
                if (!found) { strcpy(var_names[var_count], name); vars[var_count++] = v; }
            } else {
                printf("\033[31mError: Invalid LET syntax\033[0m\n");
            }
            continue;
        }

        // INPUT
        if (strncmp(line, "input ", 6) == 0) {
            char name[64];
            sscanf(line + 6, "%s", name);
            char buf[64];
            if (fgets(buf, sizeof(buf), stdin)) {
                int v = atoi(buf);
                int found = 0;
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], name) == 0) { vars[i] = v; found = 1; break; }
                }
                if (!found) { strcpy(var_names[var_count], name); vars[var_count++] = v; }
            }
            continue;
        }

        // SAY
        if (strncmp(line, "say ", 4) == 0) {
            char *arg = line + 4;
            int found = 0;
            for (int i = 0; i < var_count; i++) {
                if (strcmp(var_names[i], arg) == 0) { printf("\033[33m%d\033[0m\n", vars[i]); found = 1; break; }
            }
            if (!found) {
                int l = strlen(arg);
                if (l >= 2 && ((arg[0] == '"' && arg[l-1] == '"') || (arg[0] == '\'' && arg[l-1] == '\''))) {
                    arg[l-1] = '\0';
                    arg++;
                }
                printf("%s\n", arg);
            }
            continue;
        }

        // LEN
        if (strncmp(line, "len ", 4) == 0) {
            char *s = line + 4;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            printf("\033[33m%lu\033[0m\n", strlen(s));
            continue;
        }

        // UPPER
        if (strncmp(line, "upper ", 6) == 0) {
            char *s = line + 6;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            for (int i = 0; s[i]; i++) {
                if (s[i] >= 'a' && s[i] <= 'z') s[i] -= 32;
            }
            printf("%s\n", s);
            continue;
        }

        // МАТЕМАТИКА
        if (strncmp(line, "add ", 4) == 0 || strncmp(line, "sub ", 4) == 0 || 
            strncmp(line, "mul ", 4) == 0 || strncmp(line, "div ", 4) == 0) {
            char op[5], a[50], b[50];
            if (sscanf(line, "%s %s %s", op, a, b) == 3) {
                int va = atoi(a), vb = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) va = vars[i];
                    if (strcmp(var_names[i], b) == 0) vb = vars[i];
                }
                if (strcmp(op, "add") == 0) printf("\033[33m%d\033[0m\n", va + vb);
                else if (strcmp(op, "sub") == 0) printf("\033[33m%d\033[0m\n", va - vb);
                else if (strcmp(op, "mul") == 0) printf("\033[33m%d\033[0m\n", va * vb);
                else if (strcmp(op, "div") == 0 && vb != 0) printf("\033[33m%d\033[0m\n", va / vb);
            } else {
                printf("\033[31mError: Invalid MATH syntax\033[0m\n");
            }
            continue;
        }

        if (strncmp(line, "sqrt ", 5) == 0) {
            char a[50];
            if (sscanf(line + 5, "%s", a) == 1) {
                int va = atoi(a);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) va = vars[i];
                }
                printf("\033[33m%.2f\033[0m\n", sqrt(va));
            } else {
                printf("\033[31mError: Invalid SQRT syntax\033[0m\n");
            }
            continue;
        }

        // ОБРАБОТКА НЕИЗВЕСТНОЙ КОМАНДЫ
        printf("\033[31mError: Unknown command (Неизвестная команда): %s\033[0m\n", line);
    }

    fclose(file);
    return 0;
}

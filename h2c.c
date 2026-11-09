#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
    FILE *file = stdin;
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (!file) { printf("File not found: %s\n", argv[1]); return 1; }
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (p[0] == '\0' || p[0] == '#') continue;

        // VERSION
        if (strncmp(p, "version", 7) == 0) {
            printf("\033[33mh2c v2.4.9 (with containers)\033[0m\n");
            continue;
        }
        // SAY
        else if (strncmp(p, "say ", 4) == 0) {
            char *s = p + 4;
            if (s[0] == '"' || s[0] == '\'') s++;
            int l = strlen(s);
            if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0';
            printf("\033[37m%s\033[0m\n", s);
        }
        // LET
        else if (strncmp(p, "let ", 4) == 0) {
            // Заглушка для C-движка (просто выводим, что переменная создана)
            char var[64];
            sscanf(p + 4, "%s", var);
            printf("\033[32m[OK] Переменная %s создана\033[0m\n", var);
        }
        // INPUT
        else if (strncmp(p, "input ", 6) == 0) {
            char var[64];
            sscanf(p + 6, "%s", var);
            printf("\033[36mВведите %s: \033[0m", var);
            char buf[256];
            if (fgets(buf, sizeof(buf), stdin)) {
                printf("\033[32m[OK] Введено: %s\033[0m", buf);
            }
        }
        // MATH
        else if (strncmp(p, "add ", 4) == 0) { int a, b; sscanf(p + 4, "%d %d", &a, &b); printf("\033[33m%d + %d = %d\033[0m\n", a, b, a + b); }
        else if (strncmp(p, "sub ", 4) == 0) { int a, b; sscanf(p + 4, "%d %d", &a, &b); printf("\033[33m%d - %d = %d\033[0m\n", a, b, a - b); }
        else if (strncmp(p, "mul ", 4) == 0) { int a, b; sscanf(p + 4, "%d %d", &a, &b); printf("\033[33m%d * %d = %d\033[0m\n", a, b, a * b); }
        else if (strncmp(p, "div ", 4) == 0) { int a, b; sscanf(p + 4, "%d %d", &a, &b); if (b != 0) printf("\033[33m%d / %d = %d\033[0m\n", a, b, a / b); else printf("\033[31mОшибка: деление на ноль\033[0m\n"); }
        else if (strncmp(p, "sqrt ", 5) == 0) { double a; sscanf(p + 5, "%lf", &a); printf("\033[33m√%.2f = %.2f\033[0m\n", a, sqrt(a)); }
        // IF / END
        else if (strncmp(p, "if ", 3) == 0) { printf("\033[36m[IF] Условие: %s\033[0m\n", p + 3); }
        else if (strncmp(p, "end", 3) == 0) { printf("\033[36m[END]\033[0m\n"); }
        // LEN / UPPER
        else if (strncmp(p, "len ", 4) == 0) { char *s = p + 4; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[33mДлина: %lu\033[0m\n", strlen(s)); }
        else if (strncmp(p, "upper ", 6) == 0) { char *s = p + 6; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[33m"); for(int i=0; s[i]; i++) { if(s[i]>='a'&&s[i]<='z') printf("%c", s[i]-32); else printf("%c", s[i]); } printf("\033[0m\n"); }
        // NOW
        else if (strncmp(p, "now", 3) == 0) { time_t t = time(NULL); struct tm *tm = localtime(&t); printf("\033[33m%02d:%02d:%02d\033[0m\n", tm->tm_hour, tm->tm_min, tm->tm_sec); }
        // NET / SYS
        else if (strncmp(p, "net ", 4) == 0) { char cmd[512]; snprintf(cmd, sizeof(cmd), "curl -s %s", p + 4); system(cmd); }
        else if (strncmp(p, "sys ", 4) == 0) { system(p + 4); }
        
        // === ЦВЕТА ===
        else if (strncmp(p, "red ", 4) == 0) { char *s = p + 4; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[31m%s\033[0m\n", s); }
        else if (strncmp(p, "green ", 6) == 0) { char *s = p + 6; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[32m%s\033[0m\n", s); }
        else if (strncmp(p, "yellow ", 7) == 0) { char *s = p + 7; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[33m%s\033[0m\n", s); }
        else if (strncmp(p, "blue ", 5) == 0) { char *s = p + 5; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[34m%s\033[0m\n", s); }
        
        // НОВЫЕ ЦВЕТА
        else if (strncmp(p, "magenta ", 8) == 0) { char *s = p + 8; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[35m%s\033[0m\n", s); }
        else if (strncmp(p, "cyan ", 5) == 0) { char *s = p + 5; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[36m%s\033[0m\n", s); }
        else if (strncmp(p, "white ", 6) == 0) { char *s = p + 6; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[37m%s\033[0m\n", s); }
        else if (strncmp(p, "black ", 6) == 0) { char *s = p + 6; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[30m%s\033[0m\n", s); }

        // Неизвестная команда
        else {
            printf("\033[31mError: unknown command: %s\033[0m\n", p);
        }
    }

    if (file != stdin) fclose(file);
    return 0;
}

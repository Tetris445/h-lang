#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: hvm <file.hbc>\n"); return 1; }
    FILE *file = fopen(argv[1], "r");
    if (!file) { printf("\033[31mFile not found\033[0m\n"); return 1; }

    char line[1024];
    int vars[100];
    char var_names[100][64];
    int var_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;

        // Формат байт-кода: OPCODE ARG1 ARG2
        char op[16], a[64], b[64];
        int n = sscanf(line, "%s %s %s", op, a, b);
        if (n < 1) continue;

        if (strcmp(op, "SAY") == 0) {
            printf("%s\n", a);
        } else if (strcmp(op, "SAY_VAR") == 0) {
            for (int i = 0; i < var_count; i++) {
                if (strcmp(var_names[i], a) == 0) { printf("\033[33m%d\033[0m\n", vars[i]); break; }
            }
        } else if (strcmp(op, "LET") == 0) {
            if (n == 3) {
                int v = atoi(b);
                int found = 0;
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) { vars[i] = v; found = 1; break; }
                }
                if (!found) { strcpy(var_names[var_count], a); vars[var_count++] = v; }
            }
        } else if (strcmp(op, "ADD") == 0) {
            if (n == 3) {
                int va = atoi(a), vb = atoi(b);
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(var_names[i], a) == 0) va = vars[i];
                    if (strcmp(var_names[i], b) == 0) vb = vars[i];
                }
                printf("\033[33m%d\033[0m\n", va + vb);
            }
        } else if (strcmp(op, "EXIT") == 0) {
            break;
        } else {
            printf("\033[31mUnknown opcode: %s\033[0m\n", op);
        }
    }

    fclose(file);
    return 0;
}

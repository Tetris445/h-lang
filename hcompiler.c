#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Получить ANSI-код по имени цвета
const char* color_map(const char* name) {
    if (strcmp(name, "red") == 0) return "\033[31m";
    if (strcmp(name, "green") == 0) return "\033[32m";
    if (strcmp(name, "yellow") == 0) return "\033[33m";
    if (strcmp(name, "blue") == 0) return "\033[34m";
    if (strcmp(name, "magenta") == 0) return "\033[35m";
    if (strcmp(name, "cyan") == 0) return "\033[36m";
    if (strcmp(name, "white") == 0) return "\033[37m";
    if (strcmp(name, "black") == 0) return "\033[30m";
    return "\033[37m"; // Белый по умолчанию
}

// Смешивание двух цветов
const char* mix_colors(const char* c1, const char* c2) {
    // Если один из цветов уже смешанный - не поддерживаем (возвращаем белый)
    if (strstr(c1, "+") || strstr(c2, "+")) return "\033[37m";

    // Логика смешивания (RGB-приближение)
    if ((strcmp(c1,"red")==0 && strcmp(c2,"green")==0) || (strcmp(c1,"green")==0 && strcmp(c2,"red")==0)) return "\033[33m"; // Yellow
    if ((strcmp(c1,"red")==0 && strcmp(c2,"blue")==0) || (strcmp(c1,"blue")==0 && strcmp(c2,"red")==0)) return "\033[35m"; // Magenta
    if ((strcmp(c1,"green")==0 && strcmp(c2,"blue")==0) || (strcmp(c1,"blue")==0 && strcmp(c2,"green")==0)) return "\033[36m"; // Cyan
    if ((strcmp(c1,"red")==0 && strcmp(c2,"yellow")==0) || (strcmp(c1,"yellow")==0 && strcmp(c2,"red")==0)) return "\033[91m"; // Light Red
    if ((strcmp(c1,"red")==0 && strcmp(c2,"white")==0) || (strcmp(c1,"white")==0 && strcmp(c2,"red")==0)) return "\033[95m"; // Light Magenta
    if ((strcmp(c1,"blue")==0 && strcmp(c2,"white")==0) || (strcmp(c1,"white")==0 && strcmp(c2,"blue")==0)) return "\033[94m"; // Light Blue
    if ((strcmp(c1,"green")==0 && strcmp(c2,"white")==0) || (strcmp(c1,"white")==0 && strcmp(c2,"green")==0)) return "\033[92m"; // Light Green
    
    return "\033[37m"; // Белый, если комбинация неизвестна
}

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: hcompiler <file.h>\n"); return 1; }
    FILE *in = fopen(argv[1], "r");
    if (!in) { printf("File not found\n"); return 1; }
    
    char out_name[256];
    strcpy(out_name, argv[1]);
    char *dot = strrchr(out_name, '.');
    if (dot) *dot = '\0';
    strcat(out_name, ".c");
    
    FILE *out = fopen(out_name, "w");
    if (!out) { printf("Cannot create output file\n"); return 1; }
    
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <string.h>\n");
    fprintf(out, "#include <time.h>\n");
    fprintf(out, "#include <math.h>\n\n");
    fprintf(out, "int main() {\n");
    fprintf(out, "    void *pool = NULL;\n");
    fprintf(out, "    int used = 0;\n");
    fprintf(out, "    srand(time(NULL));\n");
    fprintf(out, "    char buf[256];\n\n");
    
    char line[1024];
    while (fgets(line, sizeof(line), in)) {
        line[strcspn(line, "\n")] = 0;
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (p[0] == '\0' || p[0] == '#') continue;
        
        // ASM
        if (strncmp(p, "asm ", 4) == 0) {
            char *code = p + 4;
            if (code[0] == '"' || code[0] == '\'') code++;
            int len = strlen(code);
            if (len > 0 && (code[len-1] == '"' || code[len-1] == '\'')) code[len-1] = '\0';
            fprintf(out, "    __asm__ volatile(\"%s\");\n", code);
        }
        // SAY
        else if (strncmp(p, "say ", 4) == 0) {
            char *text = p + 4;
            if (text[0] == '"' || text[0] == '\'') text++;
            int len = strlen(text);
            if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
            int is_var = 1;
            for (int i = 0; text[i]; i++) { if (text[i] == ' ' || text[i] == '+' || text[i] == '-') { is_var = 0; break; } }
            if (is_var && text[0] >= 'a' && text[0] <= 'z') {
                fprintf(out, "    printf(\"%%d\\n\", %s);\n", text);
            } else {
                fprintf(out, "    printf(\"%s\\n\");\n", text);
            }
        }
        // SAY_STR
        else if (strncmp(p, "say_str ", 8) == 0) {
            char *text = p + 8;
            if (text[0] == '"' || text[0] == '\'') text++;
            int len = strlen(text);
            if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
            fprintf(out, "    printf(\"%%s\\n\", %s);\n", text);
        }
        // LET
        else if (strncmp(p, "let ", 4) == 0) {
            char var[64], val[256];
            sscanf(p + 4, "%s = %[^\n]", var, val);
            if (strncmp(val, "add ", 4) == 0) {
                char a[64], b[64];
                sscanf(val + 4, "%s %s", a, b);
                fprintf(out, "    int %s = %s + %s;\n", var, a, b);
            } else if (strncmp(val, "random ", 7) == 0) {
                char min[64], max[64];
                sscanf(val + 7, "%s %s", min, max);
                fprintf(out, "    int %s = %s + rand() %% (%s - %s + 1);\n", var, min, max, min);
            } else if (strstr(val, var) != NULL) {
                fprintf(out, "    %s = %s;\n", var, val);
            } else {
                fprintf(out, "    int %s = %s;\n", var, val);
            }
        }
        // ADD / SUB / MUL / DIV
        else if (strncmp(p, "add ", 4) == 0) { char a[64], b[64]; sscanf(p + 4, "%s %s", a, b); fprintf(out, "    printf(\"%%d\\n\", %s + %s);\n", a, b); }
        else if (strncmp(p, "sub ", 4) == 0) { char a[64], b[64]; sscanf(p + 4, "%s %s", a, b); fprintf(out, "    printf(\"%%d\\n\", %s - %s);\n", a, b); }
        else if (strncmp(p, "mul ", 4) == 0) { char a[64], b[64]; sscanf(p + 4, "%s %s", a, b); fprintf(out, "    printf(\"%%d\\n\", %s * %s);\n", a, b); }
        else if (strncmp(p, "div ", 4) == 0) { char a[64], b[64]; sscanf(p + 4, "%s %s", a, b); fprintf(out, "    if (%s != 0) printf(\"%%d\\n\", %s / %s);\n", b, a, b); }
        // МАТЕМАТИКА
        else if (strncmp(p, "sin ", 4) == 0) { fprintf(out, "    printf(\"%%f\\n\", sin(%s));\n", p + 4); }
        else if (strncmp(p, "cos ", 4) == 0) { fprintf(out, "    printf(\"%%f\\n\", cos(%s));\n", p + 4); }
        else if (strncmp(p, "pow ", 4) == 0) { char a[64], b[64]; sscanf(p + 4, "%s %s", a, b); fprintf(out, "    printf(\"%%f\\n\", pow(%s, %s));\n", a, b); }
        
        // === ЦВЕТА (ОБНОВЛЕНО) ===
        else if (strncmp(p, "mix ", 4) == 0) {
            char c1[32], c2[32], text[256];
            sscanf(p + 4, "%31s %31s %[^\n]", c1, c2, text);
            // Убираем кавычки у текста
            if (text[0] == '"' || text[0] == '\'') { 
                memmove(text, text + 1, strlen(text)); 
                int l = strlen(text); 
                if (l > 0 && (text[l-1] == '"' || text[l-1] == '\'')) text[l-1] = '\0'; 
            }
            const char* code = mix_colors(c1, c2);
            fprintf(out, "    printf(\"%s%s\\033[0m\\n\");\n", code, text);
        }
        else if (strncmp(p, "red ", 4) == 0) { char *text = p + 4; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[31m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "green ", 6) == 0) { char *text = p + 6; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[32m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "yellow ", 7) == 0) { char *text = p + 7; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[33m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "blue ", 5) == 0) { char *text = p + 5; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[34m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "magenta ", 8) == 0) { char *text = p + 8; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[35m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "cyan ", 5) == 0) { char *text = p + 5; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[36m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "white ", 6) == 0) { char *text = p + 6; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[37m%s\\033[0m\\n\");\n", text); }
        else if (strncmp(p, "black ", 6) == 0) { char *text = p + 6; if (text[0] == '"' || text[0] == '\'') text++; int len = strlen(text); if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0'; fprintf(out, "    printf(\"\\033[30m%s\\033[0m\\n\");\n", text); }
        
        // ВРЕМЯ
        else if (strncmp(p, "now", 3) == 0) { fprintf(out, "    { time_t t = time(NULL); struct tm *tm = localtime(&t); printf(\"\\033[33m%%02d:%%02d:%%02d\\033[0m\\n\", tm->tm_hour, tm->tm_min, tm->tm_sec); }\n"); }
        // РАНДОМ
        else if (strncmp(p, "random ", 7) == 0) { char min[64], max[64]; sscanf(p + 7, "%s %s", min, max); fprintf(out, "    printf(\"%%d\\n\", %s + rand() %% (%s - %s + 1));\n", min, max, min); }
        // ВВОД ЧИСЛА
        else if (strncmp(p, "input ", 6) == 0) { char var[64]; sscanf(p + 6, "%s", var); fprintf(out, "    fgets(buf, sizeof(buf), stdin);\n    int %s = atoi(buf);\n", var); }
        // ВВОД СТРОКИ
        else if (strncmp(p, "input_str ", 10) == 0) { char var[64]; sscanf(p + 10, "%s", var); fprintf(out, "    fgets(buf, sizeof(buf), stdin);\n    buf[strcspn(buf, \"\\n\")] = 0;\n    char %s[256]; strcpy(%s, buf);\n", var, var); }
        // WHILE
        else if (strncmp(p, "while ", 6) == 0) { fprintf(out, "    while (%s) {\n", p + 6); }
        // IF
        else if (strncmp(p, "if ", 3) == 0) { char cond[256]; sscanf(p + 3, "%[^\n]", cond); fprintf(out, "    if (%s) {\n", cond); }
        else if (strncmp(p, "else", 4) == 0) { fprintf(out, "    } else {\n"); }
        else if (strncmp(p, "end", 3) == 0) { fprintf(out, "    }\n"); }
        // ПАМЯТЬ
        else if (strncmp(p, "mem_init ", 9) == 0) { char mb[64]; sscanf(p + 9, "%s", mb); fprintf(out, "    pool = malloc(%s * 1024 * 1024);\n    printf(\"Пул памяти создан: %s МБ\\n\");\n", mb, mb); }
        else if (strncmp(p, "mem_alloc ", 10) == 0) { char name[64], size[64]; sscanf(p + 10, "%s %s", name, size); fprintf(out, "    int *%s = (int*)((char*)pool + used);\n    used += %s * sizeof(int);\n    printf(\"Контейнер '%s' создан\\n\");\n", name, size, name); }
        else if (strncmp(p, "mem_set ", 8) == 0) { char name[64], idx[64], val[64]; sscanf(p + 8, "%s %s %s", name, idx, val); fprintf(out, "    %s[%s] = %s;\n", name, idx, val); }
        else if (strncmp(p, "mem_get ", 8) == 0) { char name[64], idx[64]; sscanf(p + 8, "%s %s", name, idx); fprintf(out, "    printf(\"%%d\\n\", %s[%s]);\n", name, idx); }
        else if (strncmp(p, "mem_status", 10) == 0) { fprintf(out, "    printf(\"Занято: %%d байт\\n\", used);\n"); }
        else if (strncmp(p, "mem_save ", 9) == 0) { char name[64]; sscanf(p + 9, "%s", name); fprintf(out, "    { FILE *f = fopen(\"%s\", \"wb\"); if (f) { fwrite(pool, 1, used, f); fclose(f); printf(\"Память сохранена в %s\\n\"); } }\n", name, name); }
        else if (strncmp(p, "mem_load ", 9) == 0) { char name[64]; sscanf(p + 9, "%s", name); fprintf(out, "    { FILE *f = fopen(\"%s\", \"rb\"); if (f) { fread(pool, 1, used, f); fclose(f); printf(\"Память загружена из %s\\n\"); } }\n", name, name); }
        // ФАЙЛЫ (через sys)
        else if (strncmp(p, "file_write ", 11) == 0) { char name[64], text[256]; sscanf(p + 11, "%s %[^\n]", name, text); fprintf(out, "    system(\"echo %s > %s\");\n", text, name); }
        else if (strncmp(p, "file_read ", 10) == 0) { char name[64]; sscanf(p + 10, "%s", name); fprintf(out, "    system(\"cat %s\");\n", name); }
        // LEN и UPPER
        else if (strncmp(p, "len ", 4) == 0) { char *s = p + 4; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; fprintf(out, "    printf(\"%%lu\\n\", strlen(\"%s\"));\n", s); }
        else if (strncmp(p, "upper ", 6) == 0) { char *s = p + 6; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; fprintf(out, "    { char tmp[256]; strcpy(tmp, \"%s\"); for (int i = 0; tmp[i]; i++) { if (tmp[i] >= 'a' && tmp[i] <= 'z') tmp[i] -= 32; } printf(\"%%s\\n\", tmp); }\n", s); }
        // NET и SYS
        else if (strncmp(p, "net ", 4) == 0) { fprintf(out, "    system(\"curl -s %s\");\n", p + 4); }
        else if (strncmp(p, "sys ", 4) == 0) { fprintf(out, "    system(\"%s\");\n", p + 4); }
    }
    
    fprintf(out, "    free(pool);\n");
    fprintf(out, "    return 0;\n");
    fprintf(out, "}\n");
    
    fclose(in);
    fclose(out);
    printf("Сгенерирован C-код: %s\n", out_name);
    return 0;
}

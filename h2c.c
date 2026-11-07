#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_VARS 100
#define MAX_CONTAINERS 50

typedef struct {
    void *pool;
    size_t total_size;
    size_t used_size;
    int initialized;
} MemPool;

typedef struct {
    char name[64];
    int *ptr;
    int size;
    int is_free;
} Container;

MemPool mem_pool = {NULL, 0, 0, 0};
Container containers[MAX_CONTAINERS];
int container_count = 0;

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: h2c <file.h>\n"); return 1; }
    FILE *file = fopen(argv[1], "r");
    if (!file) { printf("\033[31mFile not found\033[0m\n"); return 1; }

    char line[1024];
    long vars[MAX_VARS];
    char var_names[MAX_VARS][64];
    int var_count = 0;
    int skip_to_end = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;

        // --- МЕНЕДЖЕР ПАМЯТИ ---
        if (strncmp(line, "mem_init ", 9) == 0) {
            int mb; if (sscanf(line + 9, "%d", &mb) == 1) {
                if (mem_pool.initialized) free(mem_pool.pool);
                container_count = 0;
                size_t bytes = (size_t)mb * 1024 * 1024;
                mem_pool.pool = malloc(bytes);
                if (mem_pool.pool) {
                    mem_pool.total_size = bytes; mem_pool.used_size = 0; mem_pool.initialized = 1;
                    printf("\033[32mПул памяти создан: %d МБ (%zu байт)\033[0m\n", mb, bytes);
                } else { printf("\033[31mОшибка: не удалось выделить %d МБ\033[0m\n", mb); }
            }
            continue;
        }
        if (strncmp(line, "mem_status", 10) == 0) {
            if (mem_pool.initialized) {
                printf("\033[33mВсего: %zu байт | Занято: %zu байт | Свободно: %zu байт\033[0m\n", 
                    mem_pool.total_size, mem_pool.used_size, mem_pool.total_size - mem_pool.used_size);
                printf("\033[33mКонтейнеров: %d\033[0m\n", container_count);
            } else { printf("\033[31mПул памяти не создан\033[0m\n"); }
            continue;
        }
        if (strncmp(line, "mem_alloc ", 10) == 0) {
            char name[64]; int count;
            if (sscanf(line + 10, "%s %d", name, &count) == 2) {
                if (!mem_pool.initialized) { printf("\033[31mПул не создан\033[0m\n"); continue; }
                size_t bytes_needed = count * sizeof(int);
                if (mem_pool.used_size + bytes_needed > mem_pool.total_size) {
                    printf("\033[31mОшибка: не хватает памяти\033[0m\n"); continue;
                }
                if (container_count >= MAX_CONTAINERS) { printf("\033[31mСлишком много контейнеров\033[0m\n"); continue; }
                Container *c = &containers[container_count++];
                strcpy(c->name, name); c->ptr = (int*)((char*)mem_pool.pool + mem_pool.used_size);
                c->size = count; c->is_free = 0; mem_pool.used_size += bytes_needed;
                printf("\033[32mКонтейнер '%s' создан (размер: %d элементов)\033[0m\n", name, count);
            }
            continue;
        }
        if (strncmp(line, "mem_set ", 8) == 0) {
            char name[64]; int index, value;
            if (sscanf(line + 8, "%s %d %d", name, &index, &value) == 3) {
                for (int i = 0; i < container_count; i++) {
                    if (strcmp(containers[i].name, name) == 0 && !containers[i].is_free) {
                        if (index >= 0 && index < containers[i].size) { containers[i].ptr[index] = value; printf("\033[32mЗаписано %d в '%s'[%d]\033[0m\n", value, name, index); }
                        else { printf("\033[31mОшибка: индекс вне границ\033[0m\n"); }
                        break;
                    }
                }
            }
            continue;
        }
        if (strncmp(line, "mem_get ", 8) == 0) {
            char name[64]; int index;
            if (sscanf(line + 8, "%s %d", name, &index) == 2) {
                for (int i = 0; i < container_count; i++) {
                    if (strcmp(containers[i].name, name) == 0 && !containers[i].is_free) {
                        if (index >= 0 && index < containers[i].size) { printf("\033[33m%d\033[0m\n", containers[i].ptr[index]); }
                        else { printf("\033[31mОшибка: индекс вне границ\033[0m\n"); }
                        break;
                    }
                }
            }
            continue;
        }
        if (strncmp(line, "mem_free ", 9) == 0) {
            char name[64]; if (sscanf(line + 9, "%s", name) == 1) {
                for (int i = 0; i < container_count; i++) {
                    if (strcmp(containers[i].name, name) == 0) {
                        containers[i].is_free = 1;
                        printf("\033[32mКонтейнер '%s' освобожден\033[0m\n", name);
                        if (i == container_count - 1) {
                            mem_pool.used_size -= containers[i].size * sizeof(int);
                            container_count--;
                            printf("\033[32mПамять возвращена в пул\033[0m\n");
                        }
                        break;
                    }
                }
            }
            continue;
        }

        // --- СТАРОЕ ---
        if (skip_to_end) { if (strncmp(line, "end", 3) == 0) skip_to_end = 0; continue; }
        if (strncmp(line, "version", 7) == 0) { printf("\033[33mh2c v2.4.7 (with containers)\033[0m\n"); continue; }

        // IF
        if (strncmp(line, "if ", 3) == 0) {
            char a[50], op[5], b[50];
            if (sscanf(line + 3, "%s %s %s", a, op, b) == 3) {
                int va = atoi(a), vb = atoi(b);
                for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], a) == 0) va = vars[i]; if (strcmp(var_names[i], b) == 0) vb = vars[i]; }
                int result = 0;
                if (strcmp(op, ">") == 0) result = va > vb; else if (strcmp(op, "<") == 0) result = va < vb; else if (strcmp(op, "==") == 0) result = va == vb;
                if (!result) skip_to_end = 1;
            }
            continue;
        }

        // LET
        if (strncmp(line, "let ", 4) == 0) {
            char name[64], eq[5], val[64];
            if (sscanf(line + 4, "%s %s %s", name, eq, val) == 3) {
                int v = atoi(val);
                for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], val) == 0) v = vars[i]; }
                int found = 0;
                for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], name) == 0) { vars[i] = v; found = 1; break; } }
                if (!found) { strcpy(var_names[var_count], name); vars[var_count++] = v; }
            }
            continue;
        }

        // INPUT
        if (strncmp(line, "input ", 6) == 0) {
            char name[64]; sscanf(line + 6, "%s", name); char buf[64];
            if (fgets(buf, sizeof(buf), stdin)) {
                int v = atoi(buf); int found = 0;
                for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], name) == 0) { vars[i] = v; found = 1; break; } }
                if (!found) { strcpy(var_names[var_count], name); vars[var_count++] = v; }
            }
            continue;
        }

        // SAY
        if (strncmp(line, "say ", 4) == 0) {
            char *arg = line + 4; int found = 0;
            for (int i = 0; i < var_count; i++) { if (strcmp(var_names[i], arg) == 0) { printf("\033[33m%ld\033[0m\n", vars[i]); found = 1; break; } }
            if (!found) {
                int l = strlen(arg);
                if (l >= 2 && ((arg[0] == '"' && arg[l-1] == '"') || (arg[0] == '\'' && arg[l-1] == '\''))) { arg[l-1] = '\0'; arg++; }
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
            for (int i = 0; s[i]; i++) { if (s[i] >= 'a' && s[i] <= 'z') s[i] -= 32; }
            printf("%s\n", s);
            continue;
        }

        // ЦВЕТА
        if (strncmp(line, "red ", 4) == 0) { char *s = line + 4; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[31m%s\033[0m\n", s); continue; }
        if (strncmp(line, "green ", 6) == 0) { char *s = line + 6; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[32m%s\033[0m\n", s); continue; }
        if (strncmp(line, "yellow ", 7) == 0) { char *s = line + 7; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[33m%s\033[0m\n", s); continue; }
        if (strncmp(line, "blue ", 5) == 0) { char *s = line + 5; if (s[0] == '"' || s[0] == '\'') s++; int l = strlen(s); if (l > 0 && (s[l-1] == '"' || s[l-1] == '\'')) s[l-1] = '\0'; printf("\033[34m%s\033[0m\n", s); continue; }

        // СИСТЕМНОЕ
        if (strncmp(line, "now", 3) == 0) { time_t t = time(NULL); struct tm *tm = localtime(&t); printf("\033[33m%02d:%02d:%02d\033[0m\n", tm->tm_hour, tm->tm_min, tm->tm_sec); continue; }
        if (strncmp(line, "net ", 4) == 0) { char cmd[1024]; sprintf(cmd, "curl -s %s", line + 4); system(cmd); continue; }
        if (strncmp(line, "sys ", 4) == 0) { system(line + 4); continue; }

        // ОШИБКА
        printf("\033[31mError: Unknown command: %s\033[0m\n", line);
    }

    if (mem_pool.initialized) free(mem_pool.pool);
    fclose(file);
    return 0;
}

#define MAX_MEM 100
#define MAX_LINE 1024
#define MAX_DIR 4096

char mem[MAX_MEM][256];
char file_buf[4096];
char dir_buf[MAX_DIR];

// === PRINT (sys_write) ===
void print(const char *str) {
    int len = 0; while (str[len]) len++;
    register long x0 __asm__("x0") = 1;
    register long x1 __asm__("x1") = (long)str;
    register long x2 __asm__("x2") = len;
    register long x8 __asm__("x8") = 64;
    __asm__ volatile("svc #0" : : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
}

void print_int(int num) {
    char buf[32]; int i = 0;
    if (num == 0) { buf[i++] = '0'; }
    else {
        if (num < 0) { buf[i++] = '-'; num = -num; }
        char tmp[32]; int j = 0;
        while (num > 0) { tmp[j++] = '0' + (num % 10); num /= 10; }
        while (j > 0) buf[i++] = tmp[--j];
    }
    buf[i] = '\0';
    print(buf);
}

// === СТРОКИ ===
int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
void my_strcpy(char *dest, const char *src) { int i = 0; while (src[i]) { dest[i] = src[i]; i++; } dest[i] = '\0'; }
int my_atoi(const char *s) {
    int res = 0, sign = 1, i = 0;
    if (s[0] == '-') { sign = -1; i++; }
    while (s[i] >= '0' && s[i] <= '9') { res = res * 10 + (s[i] - '0'); i++; }
    return res * sign;
}

// === ЧТЕНИЕ ФАЙЛА ===
int read_file(const char *filename, char *buf, int max_len) {
    register long x0 __asm__("x0") = -100;
    register long x1 __asm__("x1") = (long)filename;
    register long x2 __asm__("x2") = 0;
    register long x8 __asm__("x8") = 56;
    __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
    int fd = (int)x0;
    if (fd < 0) return -1;

    register long x0_r __asm__("x0") = fd;
    register long x1_r __asm__("x1") = (long)buf;
    register long x2_r __asm__("x2") = max_len - 1;
    register long x8_r __asm__("x8") = 63;
    __asm__ volatile("svc #0" : "=r"(x0_r) : "r"(x0_r), "r"(x1_r), "r"(x2_r), "r"(x8_r) : "memory");
    int bytes = (int)x0_r;
    if (bytes >= 0) buf[bytes] = '\0';

    register long x0_c __asm__("x0") = fd;
    register long x8_c __asm__("x8") = 57;
    __asm__ volatile("svc #0" : : "r"(x0_c), "r"(x8_c) : "memory");

    return bytes;
}

// === EXECUTE ===
void execute(const char *line) {
    char *p = (char*)line;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    if (p[0] == '\0' || p[0] == '#') return;

    // A0
    if (p[0] == 'A' && p[1] == '0' && (p[2] == '\0' || p[2] == ' ')) {
        for (int i = 0; i < MAX_MEM; i++) mem[i][0] = '\0';
        print("[OK] A0: Память очищена\n");
    }
    // A10
    else if (p[0] == 'A' && p[1] == '1' && p[2] == '0') {
        print("[RESULT] A10: "); print(mem[1]); print("\n");
    }
    // A1
    else if (p[0] == 'A' && p[1] == '1') {
        char *text = p + 2; while (*text == ' ') text++;
        if (text[0] == '"' || text[0] == '\'') text++;
        int len = my_strlen(text);
        if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
        my_strcpy(mem[1], text);
        print("[OK] A1: Записано '"); print(mem[1]); print("' в mem[1]\n");
    }
    // A2
    else if (p[0] == 'A' && p[1] == '2') {
        char *text = p + 2; while (*text == ' ') text++;
        if (text[0] == '"' || text[0] == '\'') text++;
        int len = my_strlen(text);
        if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
        my_strcpy(mem[2], text);
        print("[OK] A2: Записано '"); print(mem[2]); print("' в mem[2]\n");
    }
    // A3
    else if (p[0] == 'A' && p[1] == '3') {
        int a = my_atoi(mem[1]), b = my_atoi(mem[2]);
        int res = a + b;
        char buf[32]; int i = 0;
        if (res == 0) buf[i++] = '0';
        else { char tmp[32]; int j = 0; while (res > 0) { tmp[j++] = '0' + (res % 10); res /= 10; } while (j > 0) buf[i++] = tmp[--j]; }
        buf[i] = '\0'; my_strcpy(mem[3], buf);
        print("[OK] A3: "); print_int(a); print(" + "); print_int(b); print(" = "); print(mem[3]); print("\n");
    }
    // C1: Создать папку
    else if (p[0] == 'C' && p[1] == '1') {
        if (mem[1][0] == '\0') {
            print("[WARN] C1: mem[1] пуста (нечего создавать)\n");
        } else {
            register long x0 __asm__("x0") = -100;
            register long x1 __asm__("x1") = (long)mem[1];
            register long x2 __asm__("x2") = 0755;
            register long x8 __asm__("x8") = 34;
            __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
            if ((int)x0 < 0) print("[ERR] C1: Не удалось создать папку\n");
            else { print("[OK] C1: Папка создана: "); print(mem[1]); print("\n"); }
        }
    }
    // C2: Удалить папку
    else if (p[0] == 'C' && p[1] == '2') {
        if (mem[1][0] == '\0') {
            print("[WARN] C2: mem[1] пуста\n");
        } else {
            register long x0 __asm__("x0") = -100;
            register long x1 __asm__("x1") = (long)mem[1];
            register long x2 __asm__("x2") = 0x200;
            register long x8 __asm__("x8") = 35;
            __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
            if ((int)x0 < 0) print("[ERR] C2: Не удалось удалить папку\n");
            else { print("[OK] C2: Папка удалена: "); print(mem[1]); print("\n"); }
        }
    }
    // C3: Переименовать папку (mem[1] -> mem[2])
    else if (p[0] == 'C' && p[1] == '3') {
        if (mem[1][0] == '\0' || mem[2][0] == '\0') {
            print("[WARN] C3: mem[1] или mem[2] пуста\n");
        } else {
            register long x0 __asm__("x0") = -100;
            register long x1 __asm__("x1") = (long)mem[1];
            register long x2 __asm__("x2") = -100;
            register long x3 __asm__("x3") = (long)mem[2];
            register long x8 __asm__("x8") = 38;
            __asm__ volatile("svc #0" : "=r"(x0) : "r"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x8) : "memory");
            if ((int)x0 < 0) print("[ERR] C3: Не удалось переименовать\n");
            else { print("[OK] C3: Переименовано '"); print(mem[1]); print("' -> '"); print(mem[2]); print("'\n"); }
        }
    }
    // Неизвестная
    else { print("[ERR] Неизвестная команда: "); print(p); print("\n"); }
}

// === ТОЧКА ВХОДА ===
void _start() {
    char *filename = "/data/data/com.termux/files/home/h-lang/vm_power.h";
    print("hvm_asm_nolibc: reading "); print(filename); print("\n");
    int bytes = read_file(filename, file_buf, sizeof(file_buf));
    if (bytes < 0) {
        print("[FATAL] File not found\n");
    } else {
        print("[OK] File read: "); print_int(bytes); print(" bytes\n");
        char *line = file_buf;
        while (*line) {
            char cmd[MAX_LINE]; int i = 0;
            while (*line && *line != '\n' && i < MAX_LINE - 1) { cmd[i++] = *line++; }
            cmd[i] = '\0';
            if (*line == '\n') line++;
            execute(cmd);
        }
    }
    register long x0 __asm__("x0") = 0;
    register long x8 __asm__("x8") = 93;
    __asm__ volatile("svc #0" : : "r"(x0), "r"(x8) : "memory");
}

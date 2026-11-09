#define MAX_MEM 100
#define MAX_LINE 1024

char mem[MAX_MEM][256];
char file_buf[4096];

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
int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) { if (a[i] != b[i]) return a[i] - b[i]; if (a[i] == '\0') return 0; }
    return 0;
}
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

// === ОЧИСТКА ===
char* skip_spaces(char *s) {
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
    return s;
}

// === EXECUTE ===
void execute(const char *line) {
    char *p = skip_spaces((char*)line);
    if (p[0] == '\0' || p[0] == '#') return;

    // A0
    if (p[0] == 'A' && p[1] == '0' && (p[2] == '\0' || p[2] == ' ')) {
        for (int i = 0; i < MAX_MEM; i++) mem[i][0] = '\0';
        print("[OK] A0: Память очищена\n");
    }
    // A1
    else if (p[0] == 'A' && p[1] == '1' && (p[2] == ' ' || p[2] == '"' || p[2] == '\'')) {
        char *text = p + 2; while (*text == ' ') text++;
        if (text[0] == '"' || text[0] == '\'') text++;
        int len = my_strlen(text);
        if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
        my_strcpy(mem[1], text);
        print("[OK] A1: Записано '"); print(mem[1]); print("' в mem[1]\n");
    }
    // A2
    else if (p[0] == 'A' && p[1] == '2' && (p[2] == ' ' || p[2] == '"' || p[2] == '\'')) {
        char *text = p + 2; while (*text == ' ') text++;
        if (text[0] == '"' || text[0] == '\'') text++;
        int len = my_strlen(text);
        if (len > 0 && (text[len-1] == '"' || text[len-1] == '\'')) text[len-1] = '\0';
        my_strcpy(mem[2], text);
        print("[OK] A2: Записано '"); print(mem[2]); print("' в mem[2]\n");
    }
    // A3
    else if (p[0] == 'A' && p[1] == '3' && (p[2] == '\0' || p[2] == ' ')) {
        int a = my_atoi(mem[1]), b = my_atoi(mem[2]);
        int res = a + b;
        char buf[32]; int i = 0;
        if (res == 0) buf[i++] = '0';
        else { char tmp[32]; int j = 0; while (res > 0) { tmp[j++] = '0' + (res % 10); res /= 10; } while (j > 0) buf[i++] = tmp[--j]; }
        buf[i] = '\0'; my_strcpy(mem[3], buf);
        print("[OK] A3: "); print_int(a); print(" + "); print_int(b); print(" = "); print(mem[3]); print("\n");
    }
    // A10
    else if (p[0] == 'A' && p[1] == '1' && p[2] == '0') {
        print("[RESULT] A10: "); print(mem[1]); print("\n");
    }
    // Неизвестная
    else { print("[ERR] Неизвестная команда: "); print(p); print("\n"); }
}

// === ТОЧКА ВХОДА ===
void _start() {
    char *filename = "/data/data/com.termux/files/home/h-lang/test_asm.h";
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

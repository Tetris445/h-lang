// hvm_asm_bridge без libc (только syscall)
#define MAX_MEM 100

char mem[MAX_MEM][256];

// === СВОЙ PRINT (через syscall write) ===
void print(const char *str) {
    int len = 0;
    while (str[len]) len++;
    register long x0 __asm__("x0") = 1;      // stdout
    register long x1 __asm__("x1") = (long)str;
    register long x2 __asm__("x2") = len;
    register long x8 __asm__("x8") = 64;     // sys_write
    __asm__ volatile("svc #0" : : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
}

// === СВОЙ PRINT_INT ===
void print_int(int num) {
    char buf[32];
    int i = 0;
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

// === СВОЙ STRLEN ===
int my_strlen(const char *s) { int len = 0; while (s[len]) len++; return len; }

// === СВОЙ STRCMP ===
int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) { if (a[i] != b[i]) return a[i] - b[i]; if (a[i] == '\0') return 0; }
    return 0;
}

// === СВОЙ STRCPY ===
void my_strcpy(char *dest, const char *src) { int i = 0; while (src[i]) { dest[i] = src[i]; i++; } dest[i] = '\0'; }

// === СВОЙ ATOI ===
int my_atoi(const char *s) {
    int res = 0, sign = 1, i = 0;
    if (s[0] == '-') { sign = -1; i++; }
    while (s[i] >= '0' && s[i] <= '9') { res = res * 10 + (s[i] - '0'); i++; }
    return res * sign;
}

// === ТОЧКА ВХОДА ===
void _start() {
    // Читаем файл (пока пропускаем, т.к. нужен sys_open)
    // Просто тестируем print:
    print("Hello from hvm_asm without libc!\n");
    print_int(42);
    print("\n");

    // Выход
    register long x0 __asm__("x0") = 0;
    register long x8 __asm__("x8") = 93; // sys_exit
    __asm__ volatile("svc #0" : : "r"(x0), "r"(x8) : "memory");
}

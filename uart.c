// uart.c — драйвер UART и вспомогательные функции
#define UART_BASE 0x09000000
#define UART_DR   ((volatile unsigned int*)(UART_BASE + 0x00))
#define UART_FR   ((volatile unsigned int*)(UART_BASE + 0x18))

void uart_putc(char c) {
    while (*UART_FR & (1 << 5));
    *UART_DR = c;
}

void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}

char uart_getc() {
    while (*UART_FR & (1 << 4));
    return (char)(*UART_DR & 0xFF);
}

void uart_putint(int num) {
    char buf[32]; int i = 0;
    if (num == 0) { uart_putc('0'); return; }
    if (num < 0) { uart_putc('-'); num = -num; }
    while (num > 0) { buf[i++] = '0' + (num % 10); num /= 10; }
    while (i > 0) uart_putc(buf[--i]);
}

// Своя strcmp
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Своя strcpy
char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

// Рисование рамки в терминале
void draw_box(int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i == 0 || i == h - 1) uart_putc('-');
            else if (j == 0 || j == w - 1) uart_putc('|');
            else uart_putc(' ');
        }
        uart_putc('\n');
    }
}
void uart_putc(char c) {
    while (*((volatile unsigned int*)(0x09000000 + 0x18)) & (1 << 5));
    *((volatile unsigned int*)(0x09000000)) = c;
}
void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}

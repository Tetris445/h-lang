// Точка входа для ARM (без libc)
void _start() {
    const char *msg = "Hello from syscall!\n";
    register long x0 __asm__("x0") = 1;      // stdout
    register long x1 __asm__("x1") = (long)msg;
    register long x2 __asm__("x2") = 20;
    register long x8 __asm__("x8") = 64;     // sys_write
    __asm__ volatile("svc #0" : : "r"(x0), "r"(x1), "r"(x2), "r"(x8) : "memory");
    
    // Выход (sys_exit)
    register long x0_exit __asm__("x0") = 0;
    register long x8_exit __asm__("x8") = 93;
    __asm__ volatile("svc #0" : : "r"(x0_exit), "r"(x8_exit) : "memory");
}

void _start() {
    long argc;
    char **argv;
    __asm__ volatile(
        "mov x29, sp\n"       // сохраняем sp в x29
        "ldr %0, [x29]\n"     // argc = [x29]
        "add %1, x29, #8\n"   // argv = x29 + 8
        : "=r"(argc), "=r"(argv)
        :
        : "x29", "memory"
    );
    char *filename = (argc > 1) ? argv[1] : "/data/data/com.termux/files/home/h-lang/test_folders.h";
    print("argc = "); print_int(argc); print("\n");
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

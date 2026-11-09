.global _start
.text
_start:
    mov x0, #0          // sum = 0
    mov x1, #0          // i = 0
    ldr x2, =10000000   // limit = 10 000 000 (через ldr)

loop:
    cmp x1, x2          // сравниваем i с limit
    b.ge end_loop       // если i >= limit, выходим

    add x0, x0, x1      // sum = sum + i
    add x1, x1, #1      // i = i + 1
    b loop              // повторяем

end_loop:
    // Завершаем программу (syscall exit)
    mov x8, #93         // номер syscall для exit
    svc #0              // вызов ядра

.section .text
.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0

    // Очистка буфера (читаем всё, что есть)
clear_uart:
    ldr x0, =0x09000000
    ldr w1, [x0, #0x18]  // Регистр флагов
    tst w1, #0x10        // Проверяем RXFE (Receive FIFO Empty)
    bne clear_done       // Если пусто — выходим
    ldrb w1, [x0]        // Читаем и выбрасываем
    b clear_uart

clear_done:
    // Приветствие
    ldr x1, =msg_hello
    mov x2, #17
    ldr x0, =0x09000000
    bl print_string

loop:
    ldr x0, =0x09000000
    ldr w1, [x0, #0x18]  // Читаем флаги
    tst w1, #0x10        // Если RXFE = 1, данных нет
    bne loop             // Ждём

    // Данные есть — читаем
    ldrb w1, [x0]

    // Проверяем, что это нормальный символ (32-126)
    cmp w1, #32
    blt loop             // Если меньше 32 — игнорируем
    cmp w1, #126
    bgt loop             // Если больше 126 — игнорируем

    // Эхо
    strb w1, [x0]
    b loop

print_string:
    mov x4, x2
    mov x5, x1
ps_loop:
    cbz x4, ps_done
    ldrb w3, [x5], #1
    strb w3, [x0]
    subs x4, x4, #1
    b ps_loop
ps_done:
    ret

.section .data
msg_hello:
    .ascii "Type something: \n"

.section .bss
    .align 16
stack_bottom:
    .skip 4096
stack_top:

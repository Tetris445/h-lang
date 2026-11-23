.section .text
.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0

    // Выводим зелёный "Result: "
    ldr x1, =color_green
    mov x2, #5
    ldr x0, =0x09000000
    bl print_string

    ldr x1, =msg_result
    mov x2, #8
    ldr x0, =0x09000000
    bl print_string

    // Выводим жёлтое число 30
    ldr x1, =color_yellow
    mov x2, #5
    ldr x0, =0x09000000
    bl print_string

    mov x1, #10
    mov x2, #20
    add x1, x1, x2
    ldr x0, =0x09000000
    bl print_number

    // Сброс цвета
    ldr x1, =color_reset
    mov x2, #4
    ldr x0, =0x09000000
    bl print_string

    // Переход на новую строку
    ldr x1, =newline
    mov x2, #1
    ldr x0, =0x09000000
    bl print_string

hang:
    b hang

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

print_number:
    mov x3, #0
    mov x4, x1
    mov w5, #10
pn_loop:
    udiv w6, w4, w5
    msub w7, w6, w5, w4
    add w7, w7, #48
    sub sp, sp, #1
    strb w7, [sp]
    add x3, x3, #1
    mov w4, w6
    cbz w4, pn_print
    b pn_loop
pn_print:
    cbz x3, pn_done
    ldrb w7, [sp], #1
    strb w7, [x0]
    sub x3, x3, #1
    b pn_print
pn_done:
    ret

.section .data
color_green:
    .ascii "\033[32m"
color_yellow:
    .ascii "\033[33m"
color_reset:
    .ascii "\033[0m"
msg_result:
    .ascii "Result: "
newline:
    .ascii "\n"

.section .bss
    .align 16
stack_bottom:
    .skip 4096
stack_top:

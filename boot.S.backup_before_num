.section .text
.global _start

_start:
    // Настраиваем стек
    ldr x0, =stack_top
    mov sp, x0

    // Адрес строки и её длина
    ldr x1, =message
    mov x2, #19

    // Адрес UART (PL011 в QEMU virt)
    ldr x0, =0x09000000

loop:
    ldrb w3, [x1], #1
    strb w3, [x0]
    subs x2, x2, #1
    bne loop

hang:
    b hang

.section .data
message:
    .ascii "Hello, bare-metal!\n"

.section .bss
    .align 16
stack_bottom:
    .skip 4096
stack_top:

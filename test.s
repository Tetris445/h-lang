.global _start
.text
_start:
    mov x0, #1      // file descriptor 1 (stdout)
    adr x1, msg     // address of the message
    mov x2, #14     // length of the message
    mov x8, #64     // syscall number for write
    svc #0          // invoke syscall

    mov x0, #0      // exit code 0
    mov x8, #93     // syscall number for exit
    svc #0          // invoke syscall

msg:
    .ascii "Hello from ASM\n"

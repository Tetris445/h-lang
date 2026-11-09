say "=== СВЯЗЬ h + hvm_asm_nolibc ==="
say "h-код:"
let x = 10
say x
say "Вызываю hvm_asm_nolibc:"
sys ./hvm_asm_nolibc test_math.h
say "h-код завершён!"

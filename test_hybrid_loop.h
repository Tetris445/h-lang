say "=== ГИБРИД: h + hvm_asm ==="
say "1. Цикл while (высокоуровневый h):"

let i = 0
while i < 3
    say "Итерация:"
    say i
    let i = i + 1
end

say "2. Вызов hvm_asm (низкоуровневый A0-A5):"
sys ./hvm_asm_bridge test_asm.h

say "3. Продолжаем h-код:"
say "Готово!"

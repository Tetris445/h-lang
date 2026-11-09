say "=== ГИБРИДНЫЙ МЕНЕДЖЕР ПАПОК ==="
say "Создаем папку 'my_new_project' через ASM VM:"

# Вызываем ассемблерную VM через sys
sys ./hvm_asm_nolibc

say "Готово! Проверяем:"
sys ls -la | grep my_new_project

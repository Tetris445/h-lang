# Калькулятор с вводом
say "=== Калькулятор ==="
skip

say "Первое число:"
let a = input()
let a_num = number a

say "Второе число:"
let b = input()
let b_num = number b

skip
say "Сумма:"
add a_num b_num

say "Разность:"
sub a_num b_num

say "Произведение:"
mul a_num b_num

say "Деление:"
div a_num b_num

skip
say "=== Готово ==="

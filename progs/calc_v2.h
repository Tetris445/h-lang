say "=== КОНСОЛЬНЫЙ КАЛЬКУЛЯТОР ==="
say "Выберите операцию:"
say "1 - Сложение"
say "2 - Вычитание"
say "3 - Умножение"

let choice = input()

say "Введите первое число:"
let num1 = input()

say "Введите второе число:"
let num2 = input()

if choice == 1
  say "Результат:"
  add num1 num2
end

if choice == 2
  say "Результат:"
  sub num1 num2
end

if choice == 3
  say "Результат:"
  mul num1 num2
end

say "Спасибо за использование!"

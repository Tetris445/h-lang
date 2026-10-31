say === ФИНАЛЬНАЯ ПРОВЕРКА h ===
green Зелёный цвет работает
red Красный цвет работает
add 10 5
sqrt 25
let x = 42
say x
let nums = [10, 20, 30]
say nums[1]

# Условия (без else, чтобы не сломать отступы)
let y = 10
if y > 5
  say Условие сработало!
end

# Циклы
say Начинаем цикл:
for i in 1 3
  add i 0
end

# Функции
func square(n)
  return n * n
end
say Результат функции:
square 5

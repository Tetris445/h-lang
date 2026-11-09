say "=== БЕЗОПАСНЫЙ КАЛЬКУЛЯТОР (h + C) ==="

say "Введи первое число:"
input a

say "Введи операцию (+, -, *, /):"
input_str op

say "Введи второе число:"
input b

# БЕЗОПАСНОСТЬ: Проверяем деление на ноль ДО вызова C
if op == "/"
  if b == 0
    say "ОШИБКА: Деление на ноль запрещено! C-программа не вызвана."
  else
    say "Результат:"
    sys ~/C-lang/c_calc 
    say a
    say op
    say b
  end
else
  say "Результат:"
  sys ~/C-lang/c_calc 
  say a
  say op
  say b
end

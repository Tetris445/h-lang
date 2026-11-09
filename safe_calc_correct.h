say "=== БЕЗОПАСНЫЙ КАЛЬКУЛЯТОР (h + C) ==="
say "Введи первое число:"
input a

say "Введи операцию (+, -, *, /):"
input_str op

say "Введи второе число:"
input b

# Проверка деления на ноль
if op == "/"
  if b == 0
    say "ОШИБКА: Деление на ноль запрещено!"
  else
    say "Результат:"
    # Передаем все аргументы в одной строке
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

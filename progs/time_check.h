include time

say "=== ПРОВЕРКА ВРЕМЕНИ ==="
say "Сейчас на часах:"
say now

say "Текущий год:"
say year

say "Введите число от 1 до 24 (текущий час):"
let hour = input()

if hour < 12
  say "Доброе утро!"
else
  say "Добрый день или вечер!"
end

say "Программа завершена."

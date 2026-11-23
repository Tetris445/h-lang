# --- Included from math.h ---
# math.h — Математика на чистом h
say "=== Math Library ==="
let a = 10
let b = 5
let sum = a + b
say "10 + 5 ="
say_int sum
let prod = a * b
say "10 * 5 ="
say_int prod
# --- End of math.h ---
say "\033[31mКрасный текст\033[0m"
say "\033[32mЗелёный текст\033[0m"
say "--------------------"
say "Все работает без C-библиотек!"

# --- Included from colors.h ---
# colors.h — Цвета на чистом h
say "=== Colors Library ==="
red "Красный текст"
green "Зелёный текст"
yellow "Жёлтый текст"
blue "Синий текст"
magenta "Пурпурный текст"
cyan "Голубой текст"
# --- End of colors.h ---
# --- Included from strings.h ---
# strings.h — Строки на чистом h
say "=== Strings Library ==="
let text = "Hello, h!"
say "Оригинал:"
say text
say "Верхний регистр:"
upper text
say text
say "Длина:"
len text
# --- End of strings.h ---
# --- Included from math.h ---
# math.h — Математическая библиотека на чистом h
say "=== Math Library ==="
let a = 5
let b = 10
let sum = a + b
say "5 + 10 ="
say sum
let prod = a * b
say "5 * 10 ="
say prod
# --- End of math.h ---
say "Все библиотеки работают!"

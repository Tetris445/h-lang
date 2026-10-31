say "=== ИГРА: УГАДАЙ ЧИСЛО ==="
say "Я загадал число от 1 до 10."
let secret = 7
say "Введи свой вариант:"
let guess = input()
let g = number guess

if g == secret
  say "Ты угадал! Это было 7."
else
  say "Не угадал. Было загадано 7."
end

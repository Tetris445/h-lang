say "=== ДЕМО NODE.JS ==="
say "Как тебя зовут?"
let name = input()
say "Привет,"
say name

say "Сколько тебе лет?"
let age = input()
let age_num = number age

if age_num >= 18
  say "Ты взрослый!"
else
  say "Ты еще молод!"
end

say "Случайное число от 1 до 10:"
random 1 10

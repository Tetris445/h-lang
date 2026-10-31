let secret = 42
say "Угадай число от 1 до 100:"
let guess = input()
let g = number guess
if g == secret
    say "Угадал!"
end
if g < secret
    say "Мало"
end
if g > secret
    say "Много"
end

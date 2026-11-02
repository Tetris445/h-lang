let secret = 42
say "Угадай число от 1 до 100:"

# Бесконечный цикл, пока не угадаем
let g = 0
while g != secret
  let guess = input()
  let g = number guess
  if g == secret
    say "Угадал!"
  else
    if g < secret
      say "Мало"
    else
      say "Много"
    end
  end
end

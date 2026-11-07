say "=== Игра: Угадай число (Node.js) ==="
let secret = random 1 10
let guess = 0
let tries = 0

while guess != secret
    say "Введи число от 1 до 10:"
    let guess = input()
    let guess = number guess
    let tries = tries + 1
    if guess < secret
        say "Мало!"
    end
    if guess > secret
        say "Много!"
    end
end

say "Угадал! Попыток:"
say tries

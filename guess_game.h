{import: "Python"}
say {"Я загадал число от 1 до 10. Угадай!"}
sys python -c "import random; print(random.randint(1,10))" > /tmp/secret.txt
input_str guess
let secret = $(cat /tmp/secret.txt)
if guess == secret
    say {"Правильно!"}
else
    say {"Неправильно, было: $secret"}
end

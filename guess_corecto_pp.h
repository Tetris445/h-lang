sys python --version
say "=== Угадай число ==="
say "Я загадал число от 1 до 10."
sys python -c "import random; print(random.randint(1,10))" > /tmp/secret.txt
let secret = $(cat /tmp/secret.txt)

say "Твой вариант:"
input_str guess

if guess == secret
    color green
say "✅ Правильно!"
    reset
else
    color red
say "❌ Неправильно. Было: $secret"
    reset
end

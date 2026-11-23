#!/bin/bash
# Игра "Угадай число" на чистом Bash

echo "=== Угадай число ==="
echo "Я загадал число от 1 до 10."

# Генерируем случайное число
SECRET=$((RANDOM % 10 + 1))

read -p "Твой вариант: " GUESS

if [ "$GUESS" -eq "$SECRET" ]; then
    echo "✅ Правильно! Это было $SECRET"
else
    echo "❌ Неправильно. Было $SECRET"
fi

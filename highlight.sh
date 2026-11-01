#!/bin/bash
FILE=$1

if [ -z "$FILE" ]; then
    echo "Использование: ./highlight.sh файл.h"
    exit 1
fi

# Используем printf для правильной интерпретации цветов
while IFS= read -r line; do
    # Заменяем слова на цветные коды
    line=$(echo "$line" | sed 's/\bsay\b/\\033[1;34msay\\033[0m/g')
    line=$(echo "$line" | sed 's/\blet\b/\\033[1;33mlet\\033[0m/g')
    line=$(echo "$line" | sed 's/\bif\b/\\033[1;31mif\\033[0m/g')
    line=$(echo "$line" | sed 's/\bend\b/\\033[1;31mend\\033[0m/g')
    line=$(echo "$line" | sed 's/\bwhile\b/\\033[1;31mwhile\\033[0m/g')
    line=$(echo "$line" | sed 's/\badd\b/\\033[1;32madd\\033[0m/g')
    line=$(echo "$line" | sed 's/\bsub\b/\\033[1;32msub\\033[0m/g')
    line=$(echo "$line" | sed 's/\bmul\b/\\033[1;32mmul\\033[0m/g')
    line=$(echo "$line" | sed 's/\bdiv\b/\\033[1;32mdiv\\033[0m/g')
    
    # Печатаем строку с интерпретацией цветов
    echo -e "$line"
done < "$FILE"

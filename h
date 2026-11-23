#!/bin/bash
# h — интерпретатор языка h на Bash (без C, без clang)

if [ -z "$1" ]; then
    echo "Usage: ./h <file.h>"
    exit 1
fi

FILE="$1"
if [ ! -f "$FILE" ]; then
    echo "File not found: $FILE"
    exit 1
fi

# Ассоциативный массив для переменных
declare -A vars

# Читаем файл построчно
while IFS= read -r line; do
    # Убираем комментарии и пробелы в начале
    line=$(echo "$line" | sed 's/#.*//' | sed 's/^[[:space:]]*//')
    [ -z "$line" ] && continue

    # SAY (вывод)
    if [[ "$line" =~ ^say[[:space:]]+(.*)$ ]]; then
        text="${BASH_REMATCH[1]}"
        # Убираем кавычки
        text=$(echo "$text" | sed 's/^"//;s/"$//;s/^'"'"'//;s/'"'"'$//')
        echo "$text"
    fi

    # LET (переменные)
    if [[ "$line" =~ ^let[[:space:]]+([a-zA-Z_][a-zA-Z0-9_]*)[[:space:]]*=[[:space:]]*(.*)$ ]]; then
        var="${BASH_REMATCH[1]}"
        val="${BASH_REMATCH[2]}"
        # Если значение содержит переменные или математику
        val=$(eval "echo $val" 2>/dev/null || echo "$val")
        vars["$var"]="$val"
        echo "[OK] Переменная $var = $val"
    fi

    # ADD (сложение)
    if [[ "$line" =~ ^add[[:space:]]+(.*)$ ]]; then
        args="${BASH_REMATCH[1]}"
        result=$(eval "echo $args" 2>/dev/null || echo "0")
        echo "[RESULT] $result"
    fi

    # SUB
    if [[ "$line" =~ ^sub[[:space:]]+(.*)$ ]]; then
        args="${BASH_REMATCH[1]}"
        result=$(eval "echo $args" 2>/dev/null || echo "0")
        echo "[RESULT] $result"
    fi

    # MUL
    if [[ "$line" =~ ^mul[[:space:]]+(.*)$ ]]; then
        args="${BASH_REMATCH[1]}"
        result=$(eval "echo $args" 2>/dev/null || echo "0")
        echo "[RESULT] $result"
    fi

    # DIV
    if [[ "$line" =~ ^div[[:space:]]+(.*)$ ]]; then
        args="${BASH_REMATCH[1]}"
        result=$(eval "echo $args" 2>/dev/null || echo "0")
        echo "[RESULT] $result"
    fi

    # IF (условие)
    if [[ "$line" =~ ^if[[:space:]]+(.*)$ ]]; then
        cond="${BASH_REMATCH[1]}"
        if eval "[ $cond ]" 2>/dev/null; then
            echo "[OK] IF выполнено"
        else
            echo "[SKIP] IF пропущено"
        fi
    fi

    # WHILE (цикл) — упрощённо
    if [[ "$line" =~ ^while[[:space:]]+(.*)$ ]]; then
        cond="${BASH_REMATCH[1]}"
        if eval "[ $cond ]" 2>/dev/null; then
            echo "[OK] WHILE выполнено (1 итерация)"
        else
            echo "[SKIP] WHILE пропущено"
        fi
    fi

    # SLEEP (задержка)
    if [[ "$line" =~ ^sleep[[:space:]]+([0-9]+)$ ]]; then
        ms="${BASH_REMATCH[1]}"
        sleep "$(echo "scale=3; $ms/1000" | bc 2>/dev/null || echo 0)"
        echo "[SLEEP] $ms ms"
    fi

    # EXIT
    if [[ "$line" == "exit" ]]; then
        echo "Bye!"
        exit 0
    fi

done < "$FILE"

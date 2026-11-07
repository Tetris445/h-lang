#!/bin/bash

# Инициализируем память (10 МБ)
./h2c << 'H_END'
mem_init 10
H_END

while true; do
    echo "==============================="
    echo "   МЕНЕДЖЕР ПАМЯТИ (h + Bash)"
    echo "==============================="
    echo "1. Создать контейнер"
    echo "2. Записать данные"
    echo "3. Прочитать данные"
    echo "4. Освободить контейнер"
    echo "5. Статус памяти"
    echo "6. Выход"
    echo "Выбери действие:"
    read choice

    case $choice in
        1)
            echo "Введи имя контейнера (например, arr):"
            read name
            echo "Введи размер:"
            read size
            # Передаем команду в h2c
            echo "mem_alloc $name $size" | ./h2c
            ;;
        2)
            echo "Введи имя контейнера:"
            read name
            echo "Введи индекс:"
            read index
            echo "Введи значение:"
            read value
            echo "mem_set $name $index $value" | ./h2c
            ;;
        3)
            echo "Введи имя контейнера:"
            read name
            echo "Введи индекс:"
            read index
            echo "mem_get $name $index" | ./h2c
            ;;
        4)
            echo "Введи имя контейнера для освобождения:"
            read name
            echo "mem_free $name" | ./h2c
            ;;
        5)
            echo "mem_status" | ./h2c
            ;;
        6)
            echo "Выход."
            break
            ;;
        *)
            echo "Неизвестная команда."
            ;;
    esac
done

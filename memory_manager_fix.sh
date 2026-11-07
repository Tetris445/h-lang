#!/bin/bash

# Создаем временный файл для команд
TEMP_FILE="temp_cmd.h"

while true; do
    echo "==============================="
    echo "   МЕНЕДЖЕР ПАМЯТИ (v2)"
    echo "==============================="
    echo "1. Инициализировать память (10 МБ)"
    echo "2. Создать контейнер"
    echo "3. Записать данные"
    echo "4. Прочитать данные"
    echo "5. Освободить контейнер"
    echo "6. Выход"
    echo "Выбери действие:"
    read choice

    case $choice in
        1)
            echo "mem_init 10" > $TEMP_FILE
            ./h2c $TEMP_FILE
            ;;
        2)
            echo "Введи имя контейнера:"
            read name
            echo "Введи размер:"
            read size
            # Создаем файл с двумя командами (инициализация + создание)
            echo "mem_init 10" > $TEMP_FILE
            echo "mem_alloc $name $size" >> $TEMP_FILE
            ./h2c $TEMP_FILE
            ;;
        3)
            echo "Введи имя контейнера:"
            read name
            echo "Введи индекс:"
            read index
            echo "Введи значение:"
            read value
            # Инициализация (так как память сбрасывается) + создание + запись
            echo "mem_init 10" > $TEMP_FILE
            echo "mem_alloc $name 100" >> $TEMP_FILE
            echo "mem_set $name $index $value" >> $TEMP_FILE
            ./h2c $TEMP_FILE
            ;;
        4)
            echo "Введи имя контейнера:"
            read name
            echo "Введи индекс:"
            read index
            # Инициализация + создание + чтение
            echo "mem_init 10" > $TEMP_FILE
            echo "mem_alloc $name 100" >> $TEMP_FILE
            echo "mem_get $name $index" >> $TEMP_FILE
            ./h2c $TEMP_FILE
            ;;
        5)
            echo "Введи имя контейнера:"
            read name
            echo "mem_init 10" > $TEMP_FILE
            echo "mem_alloc $name 100" >> $TEMP_FILE
            echo "mem_free $name" >> $TEMP_FILE
            ./h2c $TEMP_FILE
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

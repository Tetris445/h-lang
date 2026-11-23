#!/bin/bash
# os_bash.h — Мини-ОС на Bash (v0.7.0 с цветами)

echo "========================================="
echo "     hOS v0.7.0 (Bash Edition)           "
echo "========================================="
echo "Введите 'help' для списка команд."

while true; do
    echo -n "hOS> "
    read -r cmd

    if [ -z "$cmd" ]; then
        continue
    fi

    case "$cmd" in
        "help")
            echo "Команды: help, say, add, sub, mul, div, sys, color, reset, exit"
            ;;
        "say"*)
            text="${cmd#say }"
            if [ -z "$text" ]; then
                echo "[ERR] Пустой текст"
            else
                echo "$text"
            fi
            ;;
        "add"*)
            args="${cmd#add }"
            args_clean=$(echo "$args" | tr -d ' ')
            result=$(( $args_clean ))
            echo "[RESULT] $result"
            ;;
        "sub"*)
            args="${cmd#sub }"
            args_clean=$(echo "$args" | tr -d ' ')
            result=$(( $args_clean ))
            echo "[RESULT] $result"
            ;;
        "mul"*)
            args="${cmd#mul }"
            args_clean=$(echo "$args" | tr -d ' ')
            a=$(echo "$args_clean" | cut -d'*' -f1)
            b=$(echo "$args_clean" | cut -d'*' -f2)
            result=$(( a * b ))
            echo "[RESULT] $result"
            ;;
        "div"*)
            args="${cmd#div }"
            args_clean=$(echo "$args" | tr -d ' ')
            if echo "$args_clean" | grep -q "/0"; then
                echo "[ERR] Деление на ноль!"
            else
                a=$(echo "$args_clean" | cut -d'/' -f1)
                b=$(echo "$args_clean" | cut -d'/' -f2)
                result=$(( a / b ))
                echo "[RESULT] $result"
            fi
            ;;
        "sys"*)
            sys_cmd="${cmd#sys }"
            echo "[SYS] Выполняю: $sys_cmd"
            eval "$sys_cmd"
            ;;
        "color"*)
            color="${cmd#color }"
            case "$color" in
                "red") echo -e "\033[31m" ;;
                "green") echo -e "\033[32m" ;;
                "yellow") echo -e "\033[33m" ;;
                "blue") echo -e "\033[34m" ;;
                "magenta") echo -e "\033[35m" ;;
                "cyan") echo -e "\033[36m" ;;
                "white") echo -e "\033[37m" ;;
                *) echo "Неизвестный цвет: $color" ;;
            esac
            ;;
        "reset")
            echo -e "\033[0m"
            ;;
        "exit")
            echo "Bye!"
            exit 0
            ;;
        *)
            echo "Неизвестная команда: $cmd"
            ;;
    esac
done

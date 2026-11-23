#!/bin/bash
# os_bash.h — Мини-ОС на Bash (v0.7.2 с printf)

RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
CYAN='\033[36m'
RESET='\033[0m'

echo -e "${CYAN}=========================================${RESET}"
echo -e "${CYAN}     hOS v0.7.2 (Bash Edition)           ${RESET}"
echo -e "${CYAN}=========================================${RESET}"
echo -e "Введите ${YELLOW}'help'${RESET} для списка команд."

while true; do
    printf "${GREEN}hOS> ${RESET}"
    read -r cmd

    if [ -z "$cmd" ]; then
        continue
    fi

    case "$cmd" in
        "help")
            echo -e "${CYAN}Команды:${RESET} help, say, add, sub, mul, div, sys, color, reset, exit"
            ;;
        "say"*)
            text="${cmd#say }"
            if [ -z "$text" ]; then
                echo -e "${RED}[ERR] Пустой текст${RESET}"
            else
                echo "$text"
            fi
            ;;
        "add"*)
            args="${cmd#add }"
            args_clean=$(echo "$args" | tr -d ' ')
            result=$(( $args_clean ))
            echo -e "${YELLOW}[RESULT] ${result}${RESET}"
            ;;
        "sub"*)
            args="${cmd#sub }"
            args_clean=$(echo "$args" | tr -d ' ')
            result=$(( $args_clean ))
            echo -e "${YELLOW}[RESULT] ${result}${RESET}"
            ;;
        "mul"*)
            args="${cmd#mul }"
            args_clean=$(echo "$args" | tr -d ' ')
            a=$(echo "$args_clean" | cut -d'*' -f1)
            b=$(echo "$args_clean" | cut -d'*' -f2)
            result=$(( a * b ))
            echo -e "${YELLOW}[RESULT] ${result}${RESET}"
            ;;
        "div"*)
            args="${cmd#div }"
            args_clean=$(echo "$args" | tr -d ' ')
            if echo "$args_clean" | grep -q "/0"; then
                echo -e "${RED}[ERR] Деление на ноль!${RESET}"
            else
                a=$(echo "$args_clean" | cut -d'/' -f1)
                b=$(echo "$args_clean" | cut -d'/' -f2)
                result=$(( a / b ))
                echo -e "${YELLOW}[RESULT] ${result}${RESET}"
            fi
            ;;
        "sys"*)
            sys_cmd="${cmd#sys }"
            echo -e "${BLUE}[SYS] Выполняю: ${sys_cmd}${RESET}"
            eval "$sys_cmd"
            ;;
        "color"*)
            color="${cmd#color }"
            case "$color" in
                "red") echo -e "${RED}" ;;
                "green") echo -e "${GREEN}" ;;
                "yellow") echo -e "${YELLOW}" ;;
                "blue") echo -e "${BLUE}" ;;
                "magenta") echo -e "\033[35m" ;;
                "cyan") echo -e "${CYAN}" ;;
                "white") echo -e "\033[37m" ;;
                *) echo -e "${RED}Неизвестный цвет: $color${RESET}" ;;
            esac
            ;;
        "reset")
            echo -e "${RESET}"
            ;;
        "exit")
            echo -e "${CYAN}Bye!${RESET}"
            exit 0
            ;;
        *)
            echo -e "${RED}Неизвестная команда: $cmd${RESET}"
            ;;
    esac
done

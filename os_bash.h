#!/bin/bash
# hOS v1.3 — clear (с сохранением заголовка), время, файлы, память

# Цвета
RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
MAGENTA='\033[35m'
CYAN='\033[36m'
RESET='\033[0m'

CURRENT_COLOR="$RESET"

# Функция для вывода заголовка
print_header() {
    clear
    printf "${CYAN}=========================================${RESET}\n"
    printf "${CYAN}     hOS v1.3 (Time, Files, Memory)      ${RESET}\n"
    printf "${CYAN}=========================================${RESET}\n"
    printf "Введите ${YELLOW}'help'${RESET} для списка команд.\n"
}

# Инициализация
print_header

# Инициализация памяти
rm -f /tmp/hos_mem_* 2>/dev/null

while true; do
    printf "${GREEN}hOS> ${RESET}"
    read -r cmd
    if [ -z "$cmd" ]; then continue; fi

    case "$cmd" in
        "help")
            printf "${CYAN}=== Основные команды ===${RESET}\n"
            printf "  ${YELLOW}help${RESET}      — показать этот список\n"
            printf "  ${YELLOW}clear${RESET}     — очистить экран (заголовок остаётся)\n"
            printf "  ${YELLOW}say${RESET} TEXT  — вывести текст\n"
            printf "  ${YELLOW}add${RESET} A + B — сложение\n"
            printf "  ${YELLOW}sub${RESET} A - B — вычитание\n"
            printf "  ${YELLOW}mul${RESET} A * B — умножение\n"
            printf "  ${YELLOW}div${RESET} A / B — деление\n"
            printf "  ${YELLOW}sys${RESET} CMD   — выполнить системную команду\n"
            printf "  ${YELLOW}color${RESET} C   — сменить цвет (red, green, yellow, blue, magenta, cyan)\n"
            printf "  ${YELLOW}reset${RESET}     — сбросить цвет\n"
            printf "  ${YELLOW}exit${RESET}      — выйти\n"
            printf "\n"
            printf "${CYAN}=== Время ===${RESET}\n"
            printf "  ${YELLOW}now${RESET}       — текущее время (ЧЧ:ММ:СС)\n"
            printf "  ${YELLOW}date${RESET}      — текущая дата (ГГГГ-ММ-ДД)\n"
            printf "\n"
            printf "${CYAN}=== Файлы ===${RESET}\n"
            printf "  ${YELLOW}ls${RESET}        — список файлов в текущей папке\n"
            printf "  ${YELLOW}cat${RESET} FILE  — прочитать файл\n"
            printf "  ${YELLOW}rm${RESET} FILE   — удалить файл\n"
            printf "  ${YELLOW}write${RESET} FILE TEXT — записать текст в файл\n"
            printf "\n"
            printf "${CYAN}=== Память (эмуляция) ===${RESET}\n"
            printf "  ${YELLOW}mem_alloc${RESET} SIZE — выделить память (байт)\n"
            printf "  ${YELLOW}mem_set${RESET} SLOT VALUE — записать в память\n"
            printf "  ${YELLOW}mem_get${RESET} SLOT — прочитать из памяти\n"
            ;;
        "clear")
            print_header
            ;;
        "say"*) printf "${CURRENT_COLOR}${cmd#say }${RESET}\n" ;;
        "add"*)
            args="${cmd#add }"
            args_clean=$(echo "$args" | tr -d ' ')
            result=$(( $args_clean ))
            printf "${CURRENT_COLOR}[RESULT] ${result}${RESET}\n"
            ;;
        "sub"*)
            args="${cmd#sub }"
            args_clean=$(echo "$args" | tr -d ' ')
            result=$(( $args_clean ))
            printf "${CURRENT_COLOR}[RESULT] ${result}${RESET}\n"
            ;;
        "mul"*)
            args="${cmd#mul }"
            args_clean=$(echo "$args" | tr -d ' ')
            a=$(echo "$args_clean" | cut -d'*' -f1)
            b=$(echo "$args_clean" | cut -d'*' -f2)
            result=$(( a * b ))
            printf "${CURRENT_COLOR}[RESULT] ${result}${RESET}\n"
            ;;
        "div"*)
            args="${cmd#div }"
            args_clean=$(echo "$args" | tr -d ' ')
            if echo "$args_clean" | grep -q "/0"; then
                printf "${RED}[ERR] Деление на ноль!${RESET}\n"
            else
                a=$(echo "$args_clean" | cut -d'/' -f1)
                b=$(echo "$args_clean" | cut -d'/' -f2)
                result=$(( a / b ))
                printf "${CURRENT_COLOR}[RESULT] ${result}${RESET}\n"
            fi
            ;;
        "sys"*) sys_cmd="${cmd#sys }"; printf "${CYAN}[SYS] ${sys_cmd}${RESET}\n"; eval "$sys_cmd" ;;
        "color"*)
            color="${cmd#color }"
            case "$color" in
                "red") CURRENT_COLOR="$RED" ;;
                "green") CURRENT_COLOR="$GREEN" ;;
                "yellow") CURRENT_COLOR="$YELLOW" ;;
                "blue") CURRENT_COLOR="$BLUE" ;;
                "magenta") CURRENT_COLOR="$MAGENTA" ;;
                "cyan") CURRENT_COLOR="$CYAN" ;;
                *) printf "${RED}Неизвестный цвет${RESET}\n" ;;
            esac
            printf "${CURRENT_COLOR}Цвет изменён на ${color}${RESET}\n"
            ;;
        "reset") CURRENT_COLOR="$RESET"; printf "${RESET}Цвет сброшен${RESET}\n" ;;

        # === ВРЕМЯ ===
        "now") printf "${CYAN}[TIME] $(date +%H:%M:%S)${RESET}\n" ;;
        "date") printf "${CYAN}[DATE] $(date +%Y-%m-%d)${RESET}\n" ;;

        # === ФАЙЛЫ ===
        "ls") printf "${CYAN}[FILES]${RESET}\n"; ls -la ;;
        "cat"*) file="${cmd#cat }"; if [ -f "$file" ]; then printf "${CYAN}[CAT] ${file}${RESET}\n"; cat "$file"; else printf "${RED}[ERR] Файл не найден: $file${RESET}\n"; fi ;;
        "rm"*) file="${cmd#rm }"; if [ -f "$file" ]; then rm "$file"; printf "${GREEN}[OK] Удалён: $file${RESET}\n"; else printf "${RED}[ERR] Файл не найден: $file${RESET}\n"; fi ;;
        "write"*) 
            file=$(echo "${cmd#write }" | cut -d' ' -f1)
            text=$(echo "${cmd#write }" | cut -d' ' -f2-)
            echo "$text" > "$file"
            printf "${GREEN}[OK] Записано в $file${RESET}\n"
            ;;

        # === ПАМЯТЬ ===
        "mem_alloc"*) 
            size="${cmd#mem_alloc }"
            slot=0
            while [ -f "/tmp/hos_mem_$slot" ]; do slot=$((slot + 1)); done
            dd if=/dev/zero of="/tmp/hos_mem_$slot" bs=1 count="$size" 2>/dev/null
            printf "${CYAN}[MEM] Выделено $size байт, адрес (слот): $slot${RESET}\n"
            ;;
        "mem_set"*)
            args="${cmd#mem_set }"
            slot=$(echo "$args" | cut -d' ' -f1)
            value=$(echo "$args" | cut -d' ' -f2-)
            if [ -f "/tmp/hos_mem_$slot" ]; then
                echo "$value" > "/tmp/hos_mem_$slot"
                printf "${GREEN}[OK] В память $slot записано: $value${RESET}\n"
            else
                printf "${RED}[ERR] Нет памяти по слоту $slot${RESET}\n"
            fi
            ;;
        "mem_get"*)
            slot="${cmd#mem_get }"
            if [ -f "/tmp/hos_mem_$slot" ]; then
                value=$(cat "/tmp/hos_mem_$slot")
                printf "${CYAN}[MEM] Слот $slot = $value${RESET}\n"
            else
                printf "${RED}[ERR] Нет памяти по слоту $slot${RESET}\n"
            fi
            ;;

        "exit") printf "${CYAN}Bye!${RESET}\n"; exit 0 ;;
        *) printf "${RED}Неизвестная команда: $cmd${RESET}\n" ;;
    esac
done

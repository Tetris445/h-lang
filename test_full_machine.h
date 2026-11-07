say "--- ТЕСТ МАШИННОГО КОДА ---"

say "1. Математика:"
let a = 10
let b = 5
add a b
sub a b
mul a b
div a b

say "2. Строки:"
len "Hello"
upper "hello"

say "3. Ввод строки:"
say "Введи имя:"
input_str name
say "Привет, "
say_str name

say "4. Ввод числа и математика:"
say "Введи число:"
input num
let res = num + 10
say "Результат:"
say res

say "5. Условия:"
if res > 15
    green "Больше 15!"
end
if res <= 15
    red "Меньше или равно 15!"
end

say "6. Цикл while:"
let i = 0
while i < 3
    say "Итерация:"
    say i
    let i = i + 1
end

say "7. Рандом:"
random 1 100

say "8. Память:"
mem_init 10
mem_alloc arr 3
mem_set arr 0 100
mem_get arr 0
mem_status

say "9. Файлы:"
file_write test.txt "Привет из файла!"
file_read test.txt

say "10. Ассемблер (ARM):"
asm "nop"

say "11. Системная команда:"
sys echo "Привет из sys!"

say "--- ТЕСТ ЗАВЕРШЕН ---"

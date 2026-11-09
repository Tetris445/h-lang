say "1. Записываем данные в файл (через h):"
sys echo 777 > data.txt

say "2. Вызываем VM (она прочитает файл):"
sys ./hvm_asm_nolibc

say "3. Готово!"

say "1. Создаем память и записываем число:"
mem_init 5
mem_alloc arr 10
mem_set arr 0 777

say "2. Сохраняем память в файл:"
mem_save arr "my_data.bin"

say "3. Проверяем, что файл создан:"
sys ls -la my_data.bin

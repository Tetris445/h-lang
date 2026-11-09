say "1. Создаем пустой файл:"
sys touch my_data.bin

say "2. Создаем память и записываем число:"
mem_init 5
mem_alloc arr 10
mem_set arr 0 777

say "3. Сохраняем память в файл:"
mem_save arr "my_data.bin"

say "4. Проверяем размер файла:"
sys ls -la my_data.bin

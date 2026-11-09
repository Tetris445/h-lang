say "1. Создаем arr и пишем 777:"
mem_init 5
mem_alloc arr 10
mem_set arr 0 777

say "2. Сохраняем arr в файл:"
mem_save arr "transfer.bin"

say "3. Загружаем файл в тот же arr:"
mem_load arr "transfer.bin"

say "4. Читаем arr[0] (должно быть 777):"
mem_get arr 0

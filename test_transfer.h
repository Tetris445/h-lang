say "1. Создаем arr1 и пишем 777:"
mem_init 5
mem_alloc arr1 10
mem_set arr1 0 777

say "2. Сохраняем arr1 в файл:"
mem_save arr1 "transfer.bin"

say "3. Создаем пустой arr2:"
mem_alloc arr2 10

say "4. Загружаем файл в arr2:"
mem_load arr2 "transfer.bin"

say "5. Читаем arr2[0] (должно быть 777):"
mem_get arr2 0

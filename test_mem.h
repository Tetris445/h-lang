say "1. Инициализация памяти (10 МБ):"
mem_init 10

say "2. Создание контейнера 'arr' размером 5 ячеек:"
mem_alloc arr 5

say "3. Запись данных в контейнер (индексы 0, 1, 2):"
mem_set arr 0 100
mem_set arr 1 200
mem_set arr 2 300

say "4. Чтение данных из контейнера:"
say "arr[0] ="
mem_get arr 0
say "arr[1] ="
mem_get arr 1
say "arr[2] ="
mem_get arr 2

say "5. Статус памяти:"
mem_status

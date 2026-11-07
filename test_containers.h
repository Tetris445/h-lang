say "Создаем пул на 10 МБ:"
mem_init 10

say "Создаем контейнер 'arr' на 5 элементов:"
mem_alloc arr 5

say "Записываем число 100 в arr[0]:"
mem_set arr 0 100

say "Записываем число 200 в arr[1]:"
mem_set arr 1 200

say "Читаем arr[0]:"
mem_get arr 0

say "Читаем arr[1]:"
mem_get arr 1

say "Статус памяти:"
mem_status

say "Создаем пул на 10 МБ:"
mem_init 10

say "Создаем контейнер 'hp' на 1 элемент:"
mem_alloc hp 1

say "Записываем здоровье 100:"
mem_set hp 0 100

say "Сохраняем память в файл 'save.dat':"
mem_save save.dat

say "Забываем все и создаем новый пул (как при перезапуске):"
mem_init 10

say "Пытаемся прочитать 'hp' (должно быть пусто):"
mem_get hp 0

say "Загружаем память из файла 'save.dat':"
mem_load save.dat

say "Читаем здоровье из загруженной памяти:"
mem_get hp 0

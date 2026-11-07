say "Создаем пул памяти:"
mem_init 10
say "Создаем контейнер hp:"
mem_alloc hp 1
say "Записываем 100:"
mem_set hp 0 100
say "Читаем:"
mem_get hp 0
mem_status

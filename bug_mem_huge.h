say "1. Инициализация пула (1 МБ):"
mem_init 1

say "2. Пытаемся выделить контейнер на 999999999 элементов:"
mem_alloc huge 999999999

say "3. Пытаемся записать в huge[0]:"
mem_set huge 0 777

say "4. Читаем huge[0]:"
mem_get huge 0

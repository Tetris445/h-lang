say "Создаем контейнер на 3 элемента:"
mem_init 5
mem_alloc arr 3

say "Пытаемся записать в индекс 999:"
mem_set arr 999 777
say "Читаем arr[999]:"
mem_get arr 999

say "1. Создаем пул (5 МБ):"
mem_init 5

say "2. Создаем контейнер (3 элемента):"
mem_alloc arr 3

say "3. Записываем 777 в arr[0]:"
mem_set arr 0 777

say "4. Читаем arr[0]:"
mem_get arr 0

say "5. Пытаемся записать в arr[10] (выход за границы):"
mem_set arr 10 999

say "6. Освобождаем память:"
mem_free arr

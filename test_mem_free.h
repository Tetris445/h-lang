say "1. Создаем пул (5 МБ):"
mem_init 5

say "2. Создаем контейнер 'arr' (10 элементов):"
mem_alloc arr 10
mem_set arr 0 777

say "3. Читаем arr[0]:"
mem_get arr 0

say "4. Освобождаем arr:"
mem_free arr

say "5. Пытаемся прочитать arr[0] (должно быть 0 или ошибка):"
mem_get arr 0

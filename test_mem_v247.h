say "1. Инициализация:"
mem_init 10

say "2. Создание контейнера (3 элемента):"
mem_alloc arr 3

say "3. Запись в индекс 0:"
mem_set arr 0 100

say "4. Чтение arr[0]:"
mem_get arr 0

say "5. Выход за границы (индекс 10):"
mem_set arr 10 999

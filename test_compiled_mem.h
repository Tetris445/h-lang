say "=== ТЕСТ ПАМЯТИ В БИНАРНИКЕ ==="

say "1. Инициализация пула (5 МБ):"
mem_init 5

say "2. Создание контейнера 'arr' на 3 ячейки:"
mem_alloc arr 3

say "3. Запись числа 123 в arr[1]:"
mem_set arr 1 123

say "4. Чтение arr[1]:"
mem_get arr 1

say "5. Статус памяти:"
mem_status

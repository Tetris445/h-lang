say "Создаем пул на 10 МБ:"
mem_init 10

say "Создаем контейнер 'a' на 5 элементов:"
mem_alloc a 5

say "Создаем контейнер 'b' на 3 элемента:"
mem_alloc b 3

say "Статус до освобождения:"
mem_status

say "Освобождаем последний (b):"
mem_free b

say "Статус после:"
mem_status

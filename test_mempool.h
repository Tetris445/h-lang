say "Создаем пул на 200 МБ:"
mem_init 200

say "Статус до выделения:"
mem_status

say "Выделяем 1000 байт для 'a':"
mem_alloc a 1000

say "Выделяем 5000 байт для 'b':"
mem_alloc b 5000

say "Статус после выделения:"
mem_status

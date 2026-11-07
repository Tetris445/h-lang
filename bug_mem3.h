mem_init 5
mem_alloc arr 3
say "Освобождаем arr:"
mem_free arr
say "Пытаемся освободить arr еще раз:"
mem_free arr

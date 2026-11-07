say "=== ФИНАЛЬНЫЙ ТЕСТ ==="

say "1. Проверка строк:"
len "Hello"
upper "hello"

say "2. Проверка памяти:"
mem_init 10
mem_alloc arr 3
mem_set arr 0 10
mem_set arr 1 20
mem_set arr 2 30
mem_get arr 1

say "3. Проверка ошибок (индекс вне границ):"
mem_set arr 99 404

say "4. Проверка сети (HTML):"
net https://example.com

say "5. Запуск Node.js через sys:"
sys node ./h game_loop.h

say "6. Проверка, что память после Node.js жива:"
mem_get arr 2

say "=== ТЕСТ ЗАВЕРШЕН ==="

say "1. Список файлов в папке:"
sys ls -1 | head -n 5

say "2. Текущая дата:"
sys date

say "3. Запуск Node.js-версии h из C-движка:"
sys ./h test_say.h

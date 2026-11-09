say "1. Записываем данные в файл через file_write:"
file_write "test_fixed.txt" "Привет, это фикс!"

say "2. Читаем данные через file_read:"
file_read "test_fixed.txt"

say "3. Проверяем через sys:"
sys cat test_fixed.txt

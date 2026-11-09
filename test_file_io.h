say "1. Записываем данные в файл через file_write:"
file_write "test_h_file.txt" "Привет, h!"

say "2. Читаем данные через file_read:"
file_read "test_h_file.txt"

say "3. Проверяем, что файл создан (через sys):"
sys cat test_h_file.txt

say "1. Создаем пустой файл:"
sys touch compiled_data.txt

say "2. Записываем данные в файл через sys:"
sys echo "777" > compiled_data.txt

say "3. Читаем данные через sys:"
sys cat compiled_data.txt

say "4. Проверяем размер:"
sys ls -la compiled_data.txt

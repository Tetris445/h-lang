say "=== АВТО-ЧАТ (Node.js) ==="
say "Введи сообщение:"
let msg = input()

# Склеиваем ссылку и сообщение
let full_url = add "https://webhook.site/36de65e0-3b5f-4e1d-ae26-d098e01d5b1a?text=" msg

# Создаем команду для h2c
say "Создаю файл send_now.h..."
say "net "
say full_url

# К сожалению, h не умеет писать в файлы напрямую, 
# поэтому мы просто выведем готовую команду для копирования.

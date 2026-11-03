say "=== ПОЛУАВТОМАТИЧЕСКИЙ МЕССЕНДЖЕР ==="
say "Введи свой URL (например, https://httpbin.org/get?text=):"
let url = input()

say "Введи сообщение:"
let msg = input()

# Склеиваем URL и сообщение (работает в Node.js)
let full_url = add url msg

say "-----------------------------------------"
say "Скопируй эту команду и вставь в терминал:"
say "net "
say full_url
say "-----------------------------------------"

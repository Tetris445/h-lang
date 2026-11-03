say "=== МЕСТНЫЙ ЧАТ ==="
let msg1 = ""
let msg2 = ""
let msg3 = ""

while 1 == 1
  say "Ты:"
  let text = input()
  
  if text == "выход"
    say "Чат закрыт."
    let game_over = 1
    # Тут нужен break, но его нет в h, поэтому просто завершаем
    let text = "выход"
    # (в h нет break, поэтому цикл нужно ломать через условие)
  end
  
  say "Сообщение отправлено:"
  say text
end

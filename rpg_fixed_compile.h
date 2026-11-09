say "=== БИТВА С ДРАКОНОМ (Компилятор) ==="
let hp = 100
let enemy_hp = 100
let game_over = 0

while game_over == 0
  say "Твое HP:"
  say hp
  say "HP Дракона:"
  say enemy_hp
  say "1. Атака"
  say "2. Зелье (+30)"
  say "3. Выход"
  input action

  if action == 1
    let dmg = random 10 25
    say "Ты нанес урон:"
    say dmg
    let enemy_hp = enemy_hp - dmg
  end
  if action == 2
    let hp = hp + 30
    say "Лечение!"
  end
  if action == 3
    let game_over = 1
  end

  if enemy_hp > 0
    let e_dmg = random 5 15
    say "Дракон бьет на:"
    say e_dmg
    let hp = hp - e_dmg
  end

  if hp <= 0
    let game_over = 1
    say "Ты проиграл..."
  end
  if enemy_hp <= 0
    let game_over = 1
    say "Ты победил!"
  end
end

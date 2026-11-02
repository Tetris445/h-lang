say "=== БИТВА С ДРАКОНОМ ==="
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
  let action = input()

  if action == "1"
    say "Урон:"
    random 10 25
    let enemy_hp = sub enemy_hp 15
  else
    if action == "2"
      let hp = add hp 30
      say "Лечение!"
    else
      let game_over = 1
    end
  end

  if enemy_hp > 0
    say "Дракон бьет на:"
    random 5 15
    let hp = sub hp 10
  end

  if hp <= 0
    let game_over = 1
  end
  if enemy_hp <= 0
    let game_over = 1
  end
end

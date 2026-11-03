say "=== БИТВА С ДРАКОНОМ (Финал) ==="
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
    say "Введи силу удара (1-20):"
    let dmg = input()
    let dmg_num = number dmg
    say "Ты нанес урон:"
    say dmg_num
    let enemy_hp = sub enemy_hp dmg_num
  else
    if action == "2"
      let hp = add hp 30
      say "Лечение!"
    else
      let game_over = 1
    end
  end

  if enemy_hp > 0
    say "Дракон атакует! Введи его урон (1-15):"
    let edmg = input()
    let edmg_num = number edmg
    say "Дракон нанес урон:"
    say edmg_num
    let hp = sub hp edmg_num
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

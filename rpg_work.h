say "=== БИТВА С ДРАКОНОМ (v2.4.4) ==="
let hp = 100
let enemy_hp = 100
let game_over = 0

while game_over == 0
  say "Твое HP:"
  say hp
  say "HP Дракона:"
  say enemy_hp
  say "1. Атака (введи силу удара от 1 до 20)"
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
    say "Дракон бьет на:"
    # Используем random как отдельную команду для вывода
    random 5 15
    # А урон вычитаем фиксированный, чтобы игра шла
    let hp = sub hp 10
  end

  if hp <= 0
    let game_over = 1
  end
  if enemy_hp <= 0
    let game_over = 1
  end
end

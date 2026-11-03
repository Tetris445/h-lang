say "=== БИТВА С ДРАКОНОМ (через Python) ==="

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
    # Используем Python для генерации случайного числа от 10 до 25
    # Команда py выполняет код и возвращает результат
    let dmg = py import random; print(random.randint(10, 25))
    say "Урон:"
    say dmg
    let enemy_hp = sub enemy_hp dmg
  else
    if action == "2"
      let hp = add hp 30
      say "Лечение!"
    else
      let game_over = 1
    end
  end

  if enemy_hp > 0
    let e_dmg = py import random; print(random.randint(5, 15))
    let hp = sub hp e_dmg
    say "Дракон бьет на:"
    say e_dmg
  end

  if hp <= 0
    let game_over = 1
  end
  if enemy_hp <= 0
    let game_over = 1
  end
end

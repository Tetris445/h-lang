say "========================================="
say "   БИТВА С ДРАКОНОМ (RPG на h-lang)"
say "========================================="

let hp = 100
let enemy_hp = 100
let game_over = 0

say "Ты стоишь перед пещерой. Из нее вылетает дракон!"
say "Начинается бой!"

let action = ""
# Цикл боя: крутимся, пока game_over равен 0
while game_over == 0
  say "-----------------------------------------"
  say "Твое HP:"
  say hp
  say "HP Дракона:"
  say enemy_hp
  say "Что будешь делать?"
  say "1. Атаковать мечом"
  say "2. Выпить зелье (+20 HP)"
  say "3. Попытаться сбежать"
  say "Введи действие:"
  
  let action = input()
  
  # Ход игрока
  if action == "1"
    let t = now
    let dmg = add 10 div t 1000000
    if dmg > 25
      let dmg = 25
    end
    let enemy_hp = sub enemy_hp dmg
    say "Ты нанес удар мечом! Урон:"
    say dmg
  else
    if action == "2"
      let hp = add hp 20
      if hp > 100
        let hp = 100
      end
      say "Ты выпил зелье и восстановил здоровье!"
    else
      if action == "3"
        say "Ты попытался сбежать, но дракон преградил путь!"
      else
        say "Неизвестное действие. Ты замешкался."
      end
    end
  end
  
  # Ход дракона (если он еще жив)
  if enemy_hp > 0
    let t2 = now
    let e_dmg = add 5 div t2 2000000
    if e_dmg > 15
      let e_dmg = 15
    end
    let hp = sub hp e_dmg
    say "Дракон атакует! Он нанес урон:"
    say e_dmg
  end

  # Проверка на конец игры (вместо and)
  if hp <= 0
    let game_over = 1
  end
  if enemy_hp <= 0
    let game_over = 1
  end
end

# Финальный вывод
if hp <= 0
  say "========================================="
  say "Ты пал в бою. Дракон съел тебя."
else
  if enemy_hp <= 0
    say "========================================="
    say "Ты победил! Дракон повержен!"
  end
end

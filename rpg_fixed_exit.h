say "=== БИТВА С ДРАКОНОМ (Фикс выхода) ==="
let hp = 100
let enemy_hp = 100

# Цикл работает, пока оба живы (вместо game_over)
while hp > 0
  if enemy_hp > 0
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
      # Выход: обнуляем HP, чтобы цикл закончился
      let hp = 0
      say "Ты сбежал!"
    end

    if enemy_hp > 0
      if action != 3
        let e_dmg = random 5 15
        say "Дракон бьет на:"
        say e_dmg
        let hp = hp - e_dmg
      end
    end
  end
end

if hp <= 0
  say "Ты проиграл..."
end
if enemy_hp <= 0
  say "Ты победил!"
end

say === ТЕСТ БАГОВ ===

# 1. Лишний пробел
say   Привет с пробелами

# 2. Команда skip
say Строка до skip
skip
say Строка после skip

# 3. Ввод (input)
say Введи свое имя:
let name = input()
say Привет,
say name

# 4. Let внутри while
let x = 1
while x < 3
  let y = add x 10
  say y
  let x = add x 1
end

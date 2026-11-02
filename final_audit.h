say === ТЕСТ 1: МАТЕМАТИКА ===
let a = 3
let b = 3
add a b

say === ТЕСТ 2: ЦИКЛ WHILE (с let внутри) ===
let x = 1
while x < 3
  let x = x + 1
  say x
end

say === ТЕСТ 3: КОМАНДА SKIP ===
say Строка 1
skip
say Строка 2

say === ТЕСТ 4: ВВОД (input) ===
say Введите слово:
let word = input()
say Вы ввели:
say word

say === ТЕСТ 5: ПРОБЕЛЫ В НАЧАЛЕ СТРОКИ ===
  say Текст с пробелом

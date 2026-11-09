say "Генерируем огромную строку..."
let i = 0
let long_str = ""
while i < 10000
  let long_str = add long_str A
  let i = i + 1
end

say "Отправляем в C (без проверки):"
sys ~/C-lang/c_echo 
say long_str

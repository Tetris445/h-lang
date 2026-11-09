say "=== СТРЕСС-ТЕСТ: 10 000 ЧИСЕЛ ==="

say "1. Генерируем 10 000 чисел (может занять время)..."
let i = 0
while i < 10000
  let num = random 1 1000
  say num
  let i = i + 1
end

say "2. Готово!"

say "--- КАЛЬКУЛЯТОР ---"
say "Введите первое число:"
let a = input()
let num1 = number a

say "Введите операцию (+, -, *, /):"
let op = input()

say "Введите второе число:"
let b = input()
let num2 = number b

if op == "+"
  let res = add num1 num2
  say "Результат:"
  say res
else
  if op == "-"
    let res = sub num1 num2
    say "Результат:"
    say res
  else
    if op == "*"
      let res = mul num1 num2
      say "Результат:"
      say res
    else
      if op == "/"
        if num2 == 0
          say "Ошибка: деление на ноль!"
        else
          let res = div num1 num2
          say "Результат:"
          say res
        end
      else
        say "Неизвестная операция"
      end
    end
  end
end

say "Калькулятор на h"
say "Введи первое число:"
input a
say "Введи второе число:"
input b
say "Выбери операцию (add, sub, mul, div):"
let op = input()

if op == "add"
    add a b
end
if op == "sub"
    sub a b
end
if op == "mul"
    mul a b
end
if op == "div"
    div a b
end

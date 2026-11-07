say "--- ЧАСТЬ C (память, цвета, время) ---"

say "1. Математика:"
let a = 10
let b = 5
add a b
sub a b
mul a b
div a b

say "2. Цвета:"
red "Красный"
green "Зелёный"
yellow "Жёлтый"
blue "Синий"

say "3. Время:"
now

say "4. Память:"
mem_init 10
mem_alloc arr 3
mem_set arr 0 100
mem_set arr 1 200
mem_get arr 0
mem_get arr 1
mem_status

say "--- КОНЕЦ ЧАСТИ C ---"

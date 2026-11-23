clear
say "hOS v0.3 (local server)"

while 1
    say "hOS>"
    input_str cmd

    if cmd == "help"
        say "Commands: add, mul, exit"
    end

    if cmd == "add"
        say "Введите два числа:"
        input_str a
        input_str b
        let result = a + b
        say "Результат:"
        say_int result
    end

    if cmd == "mul"
        say "Введите два числа:"
        input_str a
        input_str b
        let result = a * b
        say "Результат:"
        say_int result
    end

    if cmd == "exit"
        exit
    end
end

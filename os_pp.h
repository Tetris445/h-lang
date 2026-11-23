clear
say "Welcome to hOS (with libh)!"

while 1
    color cyan
    say "===================="
    say "      hOS Menu      "
    say "===================="
    reset
    say "1. help"
    say "2. math (add 5 10)"
    say "3. colors"
    say "4. box"
    say "5. clear"
    say "6. exit"
    say "hOS>"
    input_str cmd

    if cmd == "help"
        say "Commands: help, math, colors, box, clear, exit"
    end

    if cmd == "math"
        let x = add 5 10
        say "5 + 10 ="
        say_int x
    end

    if cmd == "colors"
        color red
        say "Red text"
        color green
        say "Green text"
        color yellow
        say "Yellow text"
        reset
    end

    if cmd == "box"
        box 20 5
    end

    if cmd == "clear"
        clear
        say "Screen cleared!"
    end

    if cmd == "exit"
        exit
    end
end

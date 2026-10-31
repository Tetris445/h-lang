# Multiplication table
say "=== Multiplication Table ==="
skip

let i = 1
while i <= 5
    say "Table for " + i + ":"
    skip

    let j = 1
    while j <= 5
        mul i j
        j = j + 1
    end

    skip
    i = i + 1
end

say "=== Done ==="
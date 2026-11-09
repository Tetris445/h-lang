say "=== Гибрид: h (машинный код) + VM ==="
say "Код h:"
let x = 10
say x

say "Вызываю чистую VM (hvm_asm_nolibc):"
sys ./hvm_asm_nolibc

say "Код h завершён!"

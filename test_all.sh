#!/bin/bash
echo "=== ТЕСТ 1: Bash-ОС ==="
echo "help
say Привет
add 5 + 10
sub 10 - 5
mul 5 * 10
div 10 / 2
div 10 / 0
unknown
say
exit" | ./os_bash.h

echo ""
echo "=== ТЕСТ 2: Виртуальная машина ==="
cat << 'INNER' > vm_power.h
say "VM Test"
let x = 5
let y = 10
add 5 10
sub 10 5
mul 5 10
div 10 5
if 5 > 3
while 10 > 5
sleep 100
A1 "Hello"
A2 "World"
A3
A10
INNER
./hvm_asm_nolibc

echo ""
echo "=== ТЕСТ 3: Библиотеки ==="
cat << 'INNER' > test_libs.h
use math.h
use colors.h
say "Библиотеки подключены!"
INNER
./hprep test_libs.h
head -n 10 test_libs_pp.h

echo ""
echo "=== ТЕСТ 4: Сборка в машинный код ==="
if command -v clang &> /dev/null; then
    clang --target=aarch64-linux-gnu -nostdlib -nostartfiles -fno-pic -no-pie -nostdinc -T link.ld start.S os.c -o kernel.elf 2>&1 | head -n 5
    if [ -f kernel.elf ]; then
        echo "✅ kernel.elf собран!"
    else
        echo "❌ Ошибка сборки kernel.elf"
    fi
else
    echo "⚠️ clang не найден, пропускаем"
fi

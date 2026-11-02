const { execSync } = require('child_process');

// Функция, которая отправляет команду в C-версию h2c
function fast(команда) {
    try {
        // Вызываем h2c по полному пути
        const result = execSync(`/data/data/com.termux/files/home/h-lang/h2c -c "${команда}"`).toString();
        return result.trim();
    } catch (e) {
        return "Ошибка C: " + e.message;
    }
}

// ТЕСТ
console.log("1. Считаем на C: add 2 mul 3 4");
console.log("Результат:", fast("add 2 mul 3 4")); // Должно быть 14

console.log("2. Считаем на C: mul add 2 3 4");
console.log("Результат:", fast("mul add 2 3 4")); // Должно быть 20

console.log("3. Считаем на C: add 10 20");
console.log("Результат:", fast("add 10 20")); // Должно быть 30

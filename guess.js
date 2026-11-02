const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Загадываем случайное число от 1 до 100
const secretNumber = Math.floor(Math.random() * 100) + 1;
let attempts = 0;

console.log("=== ИГРА 'УГАДАЙ ЧИСЛО' ===");
console.log("Я загадал число от 1 до 100. Попробуй угадать!");

function askGuess() {
    rl.question('Твой вариант: ', (answer) => {
        // Превращаем текст в число (вот оно, решение проблемы!)
        const guess = Number(answer);
        attempts++;

        if (isNaN(guess)) {
            console.log("Это не число! Попробуй еще раз.");
            askGuess();
            return;
        }

        if (guess === secretNumber) {
            console.log(`🎉 Угадал! Это было число ${secretNumber}. Ты потратил ${attempts} попыток.`);
            rl.close();
        } else if (guess < secretNumber) {
            console.log("Мало! Попробуй больше.");
            askGuess();
        } else {
            console.log("Много! Попробуй меньше.");
            askGuess();
        }
    });
}

// Запускаем игру
askGuess();

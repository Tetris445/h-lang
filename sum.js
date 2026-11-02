const readline = require('readline');
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.question('Введите первое число: ', (a) => {
    rl.question('Введите второе число: ', (b) => {
        
        // Пробуем сложить БЕЗ преобразования (как в h)
        console.log("Без Number():", a + b); 
        
        // Складываем С преобразованием (правильно)
        console.log("С Number():", Number(a) + Number(b));
        
        rl.close();
    });
});


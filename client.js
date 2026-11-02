const net = require('net');

// Функция для отправки команды на C-сервер
function fast(команда) {
    return new Promise((resolve, reject) => {
        const client = net.createConnection({ port: 8081 }, () => {
            client.write(команда);
        });
        client.on('data', (data) => {
            resolve(data.toString().trim());
            client.end();
        });
        client.on('error', (err) => reject(err));
    });
}

// Тестируем
(async () => {
    console.log("1. add 2 3");
    console.log("Результат:", await fast("add 2 3")); // 5

    console.log("2. mul add 2 3 4");
    console.log("Результат:", await fast("mul add 2 3 4")); // 20

    console.log("3. add 10 20");
    console.log("Результат:", await fast("add 10 20")); // 30
})();

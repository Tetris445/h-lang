const net = require('net');

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

(async () => {
    console.log("1. Считаем на C-сервере: add 2 3");
    console.log("Результат:", await fast("add 2 3")); // Должно быть 5

    console.log("2. Считаем на C-сервере: mul add 2 3 4");
    console.log("Результат:", await fast("mul add 2 3 4")); // Должно быть 20
})();

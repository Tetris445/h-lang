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
    console.log("=== БЕНЧМАРК ===");

    // 1. Проверяем задержку на 1 команду
    const start = process.hrtime.bigint();
    const res = await fast("add 2 3");
    const end = process.hrtime.bigint();
    console.log(`1 команда (add 2 3): ${res}`);
    console.log(`Время: ${Number(end - start) / 1e6} мс`);

    // 2. Проверяем 10 команд подряд
    const start10 = process.hrtime.bigint();
    for (let i = 0; i < 10; i++) {
        await fast("add 2 3");
    }
    const end10 = process.hrtime.bigint();
    console.log(`10 команд: ${Number(end10 - start10) / 1e6} мс (в среднем ${Number(end10 - start10) / 1e7} мс на команду)`);

    // 3. Сравниваем с чистым JavaScript
    const jsStart = process.hrtime.bigint();
    let jsResult = 0;
    for (let i = 0; i < 1000000; i++) {
        jsResult += 2 + 3;
    }
    const jsEnd = process.hrtime.bigint();
    console.log(`Чистый JS (1 млн итераций): ${Number(jsEnd - jsStart) / 1e6} мс`);

    // 4. Сравниваем с C (через сервер) на 1000 команд
    const cStart = process.hrtime.bigint();
    for (let i = 0; i < 1000; i++) {
        await fast("add 2 3");
    }
    const cEnd = process.hrtime.bigint();
    console.log(`1000 команд на C-сервере: ${Number(cEnd - cStart) / 1e6} мс`);
})();

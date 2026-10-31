// h-browser.js - Браузерная версия интерпретатора h (версия 2.4.0)
// Использует prompt для ввода и hPrint для вывода на страницу.

function hTranspile(code) {
    const lines = code.split('\n');
    const out = [];
    const sp = () => '';

    for (let raw of lines) {
        let line = raw.trim();
        if (!line || line.startsWith('#')) continue;

        // Массивы
        if (line.startsWith('let ') && line.includes('[') && line.includes(']')) {
            const match = line.match(/let\s+(\w+)\s*=\s*\[(.*?)\]/);
            if (match) { out.push('let ' + match[1] + ' = [' + match[2] + '];'); continue; }
        }

        // Чтение массива
        if (line.includes('[') && line.includes(']') && !line.startsWith('let ')) {
            const match = line.match(/(\w+)\[(\d+)\]/);
            if (match) { out.push('hPrint(' + match[1] + '[' + match[2] + ']);'); continue; }
        }

        // Функции
        if (line.startsWith('func ')) {
            const match = line.match(/func\s+(\w+)\s*\((.*?)\)/);
            if (match) { out.push('function ' + match[1] + '(' + match[2] + ') {'); continue; }
        }
        if (line.startsWith('return ')) { out.push('return ' + line.slice(7) + ';'); continue; }

        // Вызов функций
        const parts = line.split(/\s+/);
        const isFunc = lines.some(l => l.trim().startsWith('func ' + parts[0] + ' ') || l.trim().startsWith('func ' + parts[0] + '('));
        if (parts.length > 1 && isFunc) {
            const args = parts.slice(1).join(', ');
            out.push('hPrint(' + parts[0] + '(' + args + '));');
            continue;
        }

        // Цикл for
        if (line.startsWith('for ')) {
            const match = line.match(/for\s+(\w+)\s+in\s+(\d+)\s+(\d+)/);
            if (match) { out.push('for (let ' + match[1] + ' = ' + match[2] + '; ' + match[1] + ' <= ' + match[3] + '; ' + match[1] + '++) {'); continue; }
        }

        // Цикл while
        if (line.startsWith('while ')) {
            out.push('while (' + line.slice(6) + ') {');
            continue;
        }

        // Вывод (умный say)
        if (line.startsWith('say ')) {
            let content = line.slice(4).trim();
            const isVar = /^[a-zA-Z_]\w*$/.test(content);
            const isNum = !isNaN(parseFloat(content)) && isFinite(content);
            const isStr = content.startsWith('"') || content.startsWith("'");
            
            if (isVar || isNum || isStr) {
                out.push('hPrint(' + content + ');');
            } else {
                out.push('hPrint("' + content.replace(/"/g, '\\"') + '");');
            }
            continue;
        }

        // Время
        if (line === 'now') { out.push('hPrint(new Date().toLocaleString());'); continue; }
        if (line === 'year') { out.push('hPrint(new Date().getFullYear());'); continue; }
        if (line === 'month') { out.push('hPrint(new Date().getMonth() + 1);'); continue; }
        if (line === 'day') { out.push('hPrint(new Date().getDate());'); continue; }

        // Арифметика
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('sub ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' - ' + p[1] + ');'); continue; }
        if (line.startsWith('mul ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' * ' + p[1] + ');'); continue; }
        if (line.startsWith('div ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' / ' + p[1] + ');'); continue; }

        // Математика
        if (line.startsWith('sqrt ')) { out.push('hPrint(Math.sqrt(' + line.slice(5) + '));'); continue; }
        if (line.startsWith('pow ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(Math.pow(' + p[0] + ', ' + p[1] + '));'); continue; }
        if (line.startsWith('sin ')) { out.push('hPrint(Math.sin(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('cos ')) { out.push('hPrint(Math.cos(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('abs ')) { out.push('hPrint(Math.abs(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('round ')) { out.push('hPrint(Math.round(' + line.slice(6) + '));'); continue; }
        if (line === 'pi') { out.push('hPrint(Math.PI);'); continue; }

        // Случайные числа
        if (line.startsWith('random ')) {
            const p = line.slice(7).split(/\s+/);
            out.push('hPrint(Math.floor(Math.random() * (' + p[1] + ' - ' + p[0] + ' + 1)) + ' + p[0] + ');');
            continue;
        }

        // Цвета
        if (line.startsWith('red ')) { out.push('hPrint("%c[31m" + ' + JSON.stringify(line.slice(4)) + ' + "%c[0m");'); continue; }
        if (line.startsWith('green ')) { out.push('hPrint("%c[32m" + ' + JSON.stringify(line.slice(6)) + ' + "%c[0m");'); continue; }
        if (line.startsWith('yellow ')) { out.push('hPrint("%c[33m" + ' + JSON.stringify(line.slice(7)) + ' + "%c[0m");'); continue; }
        if (line.startsWith('blue ')) { out.push('hPrint("%c[34m" + ' + JSON.stringify(line.slice(5)) + ' + "%c[0m");'); continue; }

        // Переменные (с input)
        if (line.startsWith('let ') && line.includes('input()')) {
            const name = line.slice(4).split('=')[0].trim();
            out.push('let ' + name + ' = prompt("Введите значение:");');
            continue;
        }
        if (line.startsWith('let ')) { out.push('let ' + line.slice(4) + ';'); continue; }

        // Логика
        if (line.startsWith('if ')) { out.push('if (' + line.slice(3) + ') {'); continue; }
        if (line === 'else') { out.push('} else {'); continue; }
        if (line === 'end') { out.push('}'); continue; }

        out.push(line + ';');
    }
    return out.join('\n');
}

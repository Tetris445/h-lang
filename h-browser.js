// h-browser.js — транспайлер h для браузера и WebView
// Версия 2.0 — с доступом к системе

const hCache = new Map();

function hTranspile(code) {
    if (hCache.has(code)) return hCache.get(code);
    const lines = code.split('\n');
    const out = [];
    let indent = 0;

    for (let raw of lines) {
        let line = raw.trim();
        if (!line || line[0] === '#') continue;
        const sp = '    '.repeat(indent);

        // === ВЫВОД ===
        if (line.startsWith('say ')) { out.push(sp + 'hPrint(' + line.slice(4) + ');'); continue; }
        if (line.startsWith('loud ')) { out.push(sp + 'hPrint(String(' + line.slice(5) + ').toUpperCase());'); continue; }
        if (line.startsWith('quiet ')) { out.push(sp + 'hPrint(String(' + line.slice(6) + ').toLowerCase());'); continue; }
        if (line === 'skip') { out.push(sp + 'hPrint("");'); continue; }

        // === ВЫХОД ===
        if (line === 'stop') { out.push(sp + 'if (typeof process !== "undefined") process.exit(0);'); continue; }

        // === АРИФМЕТИКА ===
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push(sp + 'hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('sub ')) { const p = line.slice(4).split(/\s+/); out.push(sp + 'hPrint(' + p[0] + ' - ' + p[1] + ');'); continue; }
        if (line.startsWith('mul ')) { const p = line.slice(4).split(/\s+/); out.push(sp + 'hPrint(' + p[0] + ' * ' + p[1] + ');'); continue; }
        if (line.startsWith('div ')) { const p = line.slice(4).split(/\s+/); out.push(sp + 'hPrint(' + p[0] + ' / ' + p[1] + ');'); continue; }
        if (line.startsWith('random ')) { const p = line.slice(7).split(/\s+/); out.push(sp + 'hPrint(Math.floor(Math.random() * (' + p[1] + ' - ' + p[0] + ' + 1)) + ' + p[0] + ');'); continue; }

        // === УТИЛИТЫ ===
        if (line.startsWith('len ')) { out.push(sp + 'hPrint(' + line.slice(4) + '.length);'); continue; }

        // === СИСТЕМНЫЕ (через Java-мост) ===
        if (line.startsWith('vibrate ')) { out.push(sp + 'if (typeof Android !== "undefined") Android.vibrate(' + line.slice(8) + ');'); continue; }
        if (line.startsWith('toast ')) { out.push(sp + 'if (typeof Android !== "undefined") Android.toast(' + line.slice(6) + ');'); continue; }
        if (line.startsWith('notify ')) { const p = line.slice(7); out.push(sp + 'if (typeof Android !== "undefined") Android.notify("hApp", ' + p + ');'); continue; }
        if (line === 'battery') { out.push(sp + 'if (typeof Android !== "undefined") hPrint("Батарея: " + Android.batteryLevel() + "%");'); continue; }
        if (line === 'model') { out.push(sp + 'if (typeof Android !== "undefined") hPrint(Android.deviceModel());'); continue; }

        // === ПЕРЕМЕННЫЕ ===
        if (line.startsWith('let ') && line.includes('input()')) {
            const name = line.slice(4).split('=')[0].trim();
            out.push(sp + 'let ' + name + ' = hRead();');
            continue;
        }
        if (line.startsWith('let ')) {
            let rest = line.slice(4);
            rest = rest.replace(/\bnumber\s+(\w+)/g, 'Number($1)');
            out.push(sp + 'let ' + rest + ';');
            continue;
        }

        // === УПРАВЛЕНИЕ ===
        if (line.startsWith('if ')) { out.push(sp + 'if (' + line.slice(3) + ') {'); indent++; continue; }
        if (line === 'else') { indent--; out.push(sp + '} else {'); indent++; continue; }
        if (line.startsWith('while ')) { out.push(sp + 'while (' + line.slice(6) + ') {'); indent++; continue; }
        if (line.startsWith('func ')) { out.push(sp + 'function ' + line.slice(5) + ' {'); indent++; continue; }
        if (line.startsWith('return ')) { out.push(sp + 'return ' + line.slice(7) + ';'); continue; }
        if (line === 'end') { indent--; out.push(sp + '}'); continue; }

        // === ОБЫЧНАЯ СТРОКА ===
        out.push(sp + line + ';');
    }

    const result = out.join('\n');
    hCache.set(code, result);
    return result;
}

let hOutputEl = null;

function hPrint(text) {
    if (!hOutputEl) hOutputEl = document.getElementById('h-output');
    if (hOutputEl) {
        const div = document.createElement('div');
        div.textContent = text;
        hOutputEl.appendChild(div);
    }
    console.log(text);
}

function hRead() {
    if (typeof prompt !== 'undefined') {
        const v = prompt('Ввод:');
        return v === null ? '' : v;
    }
    return '';
}

function runH() {
    const scripts = document.querySelectorAll('script[type="text/h"]');
    hOutputEl = document.getElementById('h-output');
    for (let i = 0; i < scripts.length; i++) {
        const hCode = scripts[i].textContent;
        const jsCode = hTranspile(hCode);
        try {
            eval(jsCode);
        } catch (e) {
            hPrint('Ошибка: ' + e.message);
        }
    }
}

if (typeof window !== 'undefined') {
    window.addEventListener('load', runH);
}
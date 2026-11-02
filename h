#!/usr/bin/env node
const fs = require('fs');
const vm = require('vm');
const path = require('path');

const filename = process.argv[2];
if (!filename) { console.error('Usage: h <file.h>'); process.exit(1); }
if (!fs.existsSync(filename)) { console.error('File not found: ' + filename); process.exit(1); }

const source = fs.readFileSync(filename, 'utf-8');
const commands = {};

function loadLibrary(name) {
    const libPath = path.join(__dirname, 'lib', name + '.js');
    if (!fs.existsSync(libPath)) return null;
    return require(libPath);
}

function transpile(code) {
    const lines = code.split('\n');
    const out = [];
    const sp = () => '';

    for (let raw of lines) {
        let line = raw.trim();
        if (!line || line.startsWith('#')) continue;

        // INCLUDE
        if (line.startsWith('include ')) {
            const libName = line.slice(8).trim();
            const lib = loadLibrary(libName);
            if (lib) { for (const cmd of Object.keys(lib)) commands[cmd] = lib[cmd]; }
            continue;
        }

        // МАССИВЫ
        if (line.startsWith('let ') && line.includes('[') && line.includes(']')) {
            const match = line.match(/let\s+(\w+)\s*=\s*\[(.*?)\]/);
            if (match) { out.push('var ' + match[1] + ' = [' + match[2] + '];'); continue; }
        }

        // ЧТЕНИЕ МАССИВА
        if (line.includes('[') && line.includes(']') && !line.startsWith('let ')) {
            const match = line.match(/(\w+)\[(\d+)\]/);
            if (match) { out.push('hPrint(' + match[1] + '[' + match[2] + ']);'); continue; }
        }

        // ФУНКЦИИ
        if (line.startsWith('func ')) {
            const match = line.match(/func\s+(\w+)\s*\((.*?)\)/);
            if (match) { out.push('function ' + match[1] + '(' + match[2] + ') {'); continue; }
        }
        if (line.startsWith('return ')) { out.push('return ' + line.slice(7) + ';'); continue; }

        // ВЫЗОВ ФУНКЦИЙ (без скобок)
        const parts = line.split(/\s+/);
        const isFunc = lines.some(l => l.trim().startsWith('func ' + parts[0] + ' ') || l.trim().startsWith('func ' + parts[0] + '('));
        if (isFunc && parts.length >= 1) {
            const args = parts.length > 1 ? parts.slice(1).join(', ') : '';
            out.push(parts[0] + '(' + args + ');');
            continue;
        }

        // ЦИКЛ FOR
        if (line.startsWith('for ')) {
            const match = line.match(/for\s+(\w+)\s+in\s+(\d+)\s+(\d+)/);
            if (match) { out.push('for (var ' + match[1] + ' = ' + match[2] + '; ' + match[1] + ' <= ' + match[3] + '; ' + match[1] + '++) {'); continue; }
        }

        // ЦИКЛ WHILE
        if (line.startsWith('while ')) {
            out.push('while (' + line.slice(6) + ') {');
            continue;
        }

        // ВЫВОД (умный say)
        if (line.startsWith('say ')) {
            let content = line.slice(4).trim();
            if (commands[content]) {
                out.push('hPrint(commands["' + content + '"]());');
            } else {
                const isVar = /^[a-zA-Z_]\w*$/.test(content);
                const isNum = !isNaN(parseFloat(content)) && isFinite(content);
                const isStr = content.startsWith('"') || content.startsWith("'");
                if (isVar || isNum || isStr) {
                    out.push('hPrint(' + content + ');');
                } else {
                    out.push('hPrint("' + content.replace(/"/g, '\\"') + '");');
                }
            }
            continue;
        }

        if (line === 'skip') { out.push('hPrint("");'); continue; }
        if (line === 'now') { out.push('hPrint(new Date().toLocaleString());'); continue; }
        if (line === 'year') { out.push('hPrint(new Date().getFullYear());'); continue; }
        if (line === 'month') { out.push('hPrint(new Date().getMonth() + 1);'); continue; }
        if (line === 'day') { out.push('hPrint(new Date().getDate());'); continue; }

        // АРИФМЕТИКА
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('sub ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' - ' + p[1] + ');'); continue; }
        if (line.startsWith('mul ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' * ' + p[1] + ');'); continue; }
        if (line.startsWith('div ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(' + p[0] + ' / ' + p[1] + ');'); continue; }

        // МАТЕМАТИКА
        if (line.startsWith('sqrt ')) { out.push('hPrint(Math.sqrt(' + line.slice(5) + '));'); continue; }
        if (line.startsWith('pow ')) { const p = line.slice(4).split(/\s+/); out.push('hPrint(Math.pow(' + p[0] + ', ' + p[1] + '));'); continue; }
        if (line.startsWith('sin ')) { out.push('hPrint(Math.sin(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('cos ')) { out.push('hPrint(Math.cos(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('abs ')) { out.push('hPrint(Math.abs(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('floor ')) { out.push('hPrint(Math.floor(' + line.slice(6) + '));'); continue; }
        if (line.startsWith('ceil ')) { out.push('hPrint(Math.ceil(' + line.slice(5) + '));'); continue; }
        if (line.startsWith('round ')) { out.push('hPrint(Math.round(' + line.slice(6) + '));'); continue; }
        if (line === 'pi') { out.push('hPrint(Math.PI);'); continue; }

        // СЛУЧАЙНЫЕ ЧИСЛА
        if (line.startsWith('random ')) {
            const p = line.slice(7).split(/\s+/);
            out.push('hPrint(Math.floor(Math.random() * (' + p[1] + ' - ' + p[0] + ' + 1)) + ' + p[0] + ');');
            continue;
        }

        // ЦВЕТА
        if (line.startsWith('red ')) { out.push('hPrint("\\x1b[31m" + ' + JSON.stringify(line.slice(4)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('green ')) { out.push('hPrint("\\x1b[32m" + ' + JSON.stringify(line.slice(6)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('yellow ')) { out.push('hPrint("\\x1b[33m" + ' + JSON.stringify(line.slice(7)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('blue ')) { out.push('hPrint("\\x1b[34m" + ' + JSON.stringify(line.slice(5)) + ' + "\\x1b[0m");'); continue; }

        // ПЕРЕМЕННЫЕ
        if (line.startsWith('let ') && line.includes('input()')) {
            const name = line.slice(4).split('=')[0].trim();
            out.push('var ' + name + ' = hRead();');
            continue;
        }
        if (line.startsWith('let ') && line.includes('func ')) { const parts = line.slice(4).split('='); const name = parts[0].trim(); const call = parts[1].trim().split(/\s+/); out.push('var ' + name + ' = ' + call[0] + '(' + call.slice(1).join(', ') + ');'); continue; }
        if (line.startsWith("let ") && line.includes(" = ") && line.split(" = ")[1].includes(" ")) { const parts = line.slice(4).split(" = "); const name = parts[0].trim(); const call = parts[1].trim().split(/\s+/); out.push("var " + name + " = " + call[0] + "(" + call.slice(1).join(", ") + ");"); continue; }
        if (line.startsWith('let ')) {
            let rest = line.slice(4);
            rest = rest.replace(/number\s+(\w+)/g, 'Number($1)')
                       .replace(/add\s+(\w+)\s+(\w+)/g, '$1 + $2')
                       .replace(/sub\s+(\w+)\s+(\w+)/g, '$1 - $2')
                       .replace(/mul\s+(\w+)\s+(\w+)/g, '$1 * $2')
                       .replace(/div\s+(\w+)\s+(\w+)/g, '$1 / $2');
            out.push('var ' + rest + ';');
            continue;
        }

        // ЛОГИКА
        if (line.startsWith('if ')) { out.push('if (' + line.slice(3) + ') {'); continue; }
        if (line === 'else') { out.push('} else {'); continue; }
        if (line === 'end') { out.push('}'); continue; }

        out.push(line + ';');
    }
    return out.join('\n');
}

function hPrint(text) { console.log(text); }
function hRead() {
    try {
        const buf = Buffer.alloc(1024);
        const bytes = fs.readSync(0, buf, 0, buf.length, null);
        return buf.toString('utf-8', 0, bytes).trim();
    } catch (e) { return ''; }
}

const jsCode = transpile(source);
const context = { console, process, os: require('os'), fs, hPrint, hRead, commands, Math, require };
try { vm.createContext(context); vm.runInContext(jsCode, context); } catch (e) { console.error('Runtime error: ' + e.message); }

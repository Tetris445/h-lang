#!/usr/bin/env node
const fs = require('fs');
const vm = require('vm');
const os = require('os');
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

        // 1. INCLUDE
        if (line.startsWith('include ')) {
            const libName = line.slice(8).trim();
            const lib = loadLibrary(libName);
            if (lib) { for (const cmd of Object.keys(lib)) commands[cmd] = lib[cmd]; }
            continue;
        }

        // 2. МАССИВЫ
        if (line.startsWith('let ') && line.includes('[') && line.includes(']')) {
            const match = line.match(/let\s+(\w+)\s*=\s*\[(.*?)\]/);
            if (match) { out.push(sp() + 'let ' + match[1] + ' = [' + match[2] + '];'); continue; }
        }

        // 3. ЧТЕНИЕ МАССИВА
        if (line.includes('[') && line.includes(']') && !line.startsWith('let ')) {
            const match = line.match(/(\w+)\[(\d+)\]/);
            if (match) { out.push(sp() + 'hPrint(' + match[1] + '[' + match[2] + ']);'); continue; }
        }

        // 4. ФУНКЦИИ
        if (line.startsWith('func ')) {
            const match = line.match(/func\s+(\w+)\s*\((.*?)\)/);
            if (match) { out.push(sp() + 'function ' + match[1] + '(' + match[2] + ') {'); continue; }
        }
        if (line.startsWith('return ')) { out.push(sp() + 'return ' + line.slice(7) + ';'); continue; }

        // 5. ВЫЗОВ ФУНКЦИИ (без скобок)
        const parts = line.split(/\s+/);
        const isFunc = lines.some(l => l.trim().startsWith('func ' + parts[0] + ' ') || l.trim().startsWith('func ' + parts[0] + '('));
        if (parts.length > 1 && isFunc) {
            const args = parts.slice(1).join(', ');
            out.push(sp() + 'hPrint(' + parts[0] + '(' + args + '));');
            continue;
        }

        // 6. ЦИКЛ FOR
        if (line.startsWith('for ')) {
            const match = line.match(/for\s+(\w+)\s+in\s+(\d+)\s+(\d+)/);
            if (match) { out.push(sp() + 'for (let ' + match[1] + ' = ' + match[2] + '; ' + match[1] + ' <= ' + match[3] + '; ' + match[1] + '++) {'); continue; }
        }

        // 7. ВЫВОД
        if (line.startsWith('say ')) {
            const content = line.slice(4).trim();
            if (commands[content]) {
                out.push(sp() + 'hPrint(commands["' + content + '"]());');
            } else {
                out.push(sp() + 'hPrint(' + content + ');');
            }
            continue;
        }

        // 8. ВРЕМЯ
        if (line === 'now') { out.push(sp() + 'hPrint(new Date().toLocaleString());'); continue; }
        if (line === 'year') { out.push(sp() + 'hPrint(new Date().getFullYear());'); continue; }
        if (line === 'month') { out.push(sp() + 'hPrint(new Date().getMonth() + 1);'); continue; }
        if (line === 'day') { out.push(sp() + 'hPrint(new Date().getDate());'); continue; }

        // 9. АРИФМЕТИКА
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('sub ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' - ' + p[1] + ');'); continue; }
        if (line.startsWith('mul ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' * ' + p[1] + ');'); continue; }
        if (line.startsWith('div ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' / ' + p[1] + ');'); continue; }

        // 10. МАТЕМАТИКА
        if (line.startsWith('sqrt ')) { out.push(sp() + 'hPrint(Math.sqrt(' + line.slice(5) + '));'); continue; }
        if (line.startsWith('pow ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(Math.pow(' + p[0] + ', ' + p[1] + '));'); continue; }
        if (line.startsWith('sin ')) { out.push(sp() + 'hPrint(Math.sin(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('cos ')) { out.push(sp() + 'hPrint(Math.cos(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('abs ')) { out.push(sp() + 'hPrint(Math.abs(' + line.slice(4) + '));'); continue; }
        if (line.startsWith('floor ')) { out.push(sp() + 'hPrint(Math.floor(' + line.slice(6) + '));'); continue; }
        if (line.startsWith('ceil ')) { out.push(sp() + 'hPrint(Math.ceil(' + line.slice(5) + '));'); continue; }
        if (line.startsWith('round ')) { out.push(sp() + 'hPrint(Math.round(' + line.slice(6) + '));'); continue; }
        if (line === 'pi') { out.push(sp() + 'hPrint(Math.PI);'); continue; }

        // 11. ЦВЕТА
        if (line.startsWith('red ')) { out.push(sp() + 'hPrint("\\x1b[31m" + ' + JSON.stringify(line.slice(4)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('green ')) { out.push(sp() + 'hPrint("\\x1b[32m" + ' + JSON.stringify(line.slice(6)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('yellow ')) { out.push(sp() + 'hPrint("\\x1b[33m" + ' + JSON.stringify(line.slice(7)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('blue ')) { out.push(sp() + 'hPrint("\\x1b[34m" + ' + JSON.stringify(line.slice(5)) + ' + "\\x1b[0m");'); continue; }

        // 12. ПЕРЕМЕННЫЕ (с input)
        if (line.startsWith('let ') && line.includes('input()')) {
            const name = line.slice(4).split('=')[0].trim();
            out.push(sp() + 'let ' + name + ' = hRead();');
            continue;
        }
        if (line.startsWith('let ')) { out.push(sp() + 'let ' + line.slice(4) + ';'); continue; }

        // 13. ЛОГИКА (без indent)
        if (line.startsWith('if ')) { out.push(sp() + 'if (' + line.slice(3) + ') {'); continue; }
        if (line === 'else') { out.push(sp() + '} else {'); continue; }
        if (line.startsWith('while ')) { out.push(sp() + 'while (' + line.slice(6) + ') {'); continue; }
        if (line === 'end') { out.push(sp() + '}'); continue; }

        out.push(sp() + line + ';');
    }
    return out.join('\n');
}

function hPrint(text) { console.log(text); }

// Синхронное чтение с клавиатуры для input()
function hRead() {
    try {
        const buf = Buffer.alloc(1024);
        const bytes = fs.readSync(0, buf, 0, buf.length, null);
        return buf.toString('utf-8', 0, bytes).trim();
    } catch (e) {
        return '';
    }
}

const jsCode = transpile(source);
const context = { console: console, process: process, os: os, fs: fs, hPrint: hPrint, hRead: hRead, commands: commands, Math: Math };
try { vm.createContext(context); vm.runInContext(jsCode, context); } catch (e) { console.error('Runtime error: ' + e.message); }

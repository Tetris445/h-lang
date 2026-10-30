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
let customFuncs = [];

function loadLibrary(name) {
    const libPath = path.join(__dirname, 'lib', name + '.js');
    if (!fs.existsSync(libPath)) return null;
    return require(libPath);
}

function transpile(code) {
    const lines = code.split('\n');
    const out = [];
    let indent = 0;
    const sp = () => '    '.repeat(Math.max(0, indent));
    
    // Сначала собираем имена всех функций
    for (let raw of lines) {
        const m = raw.trim().match(/^func\s+(\w+)/);
        if (m) customFuncs.push(m[1]);
    }

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
            if (match) { out.push(sp() + 'let ' + match[1] + ' = [' + match[2] + '];'); continue; }
        }

        // ЧТЕНИЕ МАССИВА
        if (line.includes('[') && line.includes(']') && !line.startsWith('let ')) {
            const match = line.match(/(\w+)\[(\d+)\]/);
            if (match) { out.push(sp() + 'hPrint(' + match[1] + '[' + match[2] + ']);'); continue; }
        }

        // ОПРЕДЕЛЕНИЕ ФУНКЦИИ
        if (line.startsWith('func ')) {
            const match = line.match(/func\s+(\w+)\s*\((.*?)\)/);
            if (match) { out.push(sp() + 'function ' + match[1] + '(' + match[2] + ') {'); indent++; continue; }
        }
        if (line.startsWith('return ')) { out.push(sp() + 'return ' + line.slice(7) + ';'); continue; }

        // ВЫЗОВ ФУНКЦИИ (ВНУТРИ SAY)
        if (line.startsWith('say ')) {
            const content = line.slice(4).trim();
            const parts = content.split(/\s+/);
            if (parts.length > 1 && customFuncs.includes(parts[0])) {
                const args = parts.slice(1).join(', ');
                out.push(sp() + 'hPrint(' + parts[0] + '(' + args + '));');
            } else if (parts.length === 3 && ['add','sub','mul','div'].includes(parts[0])) {
                const op = parts[0]==='add'?'+':parts[0]==='sub'?'-':parts[0]==='mul'?'*':'/';
                out.push(sp() + 'hPrint(' + parts[1] + ' ' + op + ' ' + parts[2] + ');');
            } else {
                out.push(sp() + 'hPrint(' + JSON.stringify(content) + ');');
            }
            continue;
        }

        // ЦИКЛ FOR
        if (line.startsWith('for ')) {
            const match = line.match(/for\s+(\w+)\s+in\s+(\d+)\s+(\d+)/);
            if (match) { out.push(sp() + 'for (let ' + match[1] + ' = ' + match[2] + '; ' + match[1] + ' <= ' + match[3] + '; ' + match[1] + '++) {'); indent++; continue; }
        }

        // БАЗОВЫЕ
        if (line.startsWith('red ')) { out.push(sp() + 'hPrint("\\x1b[31m" + ' + JSON.stringify(line.slice(4)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('green ')) { out.push(sp() + 'hPrint("\\x1b[32m" + ' + JSON.stringify(line.slice(6)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('yellow ')) { out.push(sp() + 'hPrint("\\x1b[33m" + ' + JSON.stringify(line.slice(7)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('blue ')) { out.push(sp() + 'hPrint("\\x1b[34m" + ' + JSON.stringify(line.slice(5)) + ' + "\\x1b[0m");'); continue; }

        // ПЕРЕМЕННЫЕ
        if (line.startsWith('let ')) { out.push(sp() + line.slice(4) + ';'); continue; }

        // ЛОГИКА
        if (line.startsWith('if ')) { out.push(sp() + 'if (' + line.slice(3) + ') {'); indent++; continue; }
        if (line === 'else') { if(indent > 0) indent--; out.push(sp() + '} else {'); indent++; continue; }
        if (line.startsWith('while ')) { out.push(sp() + 'while (' + line.slice(6) + ') {'); indent++; continue; }
        if (line === 'end') { if(indent > 0) indent--; out.push(sp() + '}'); continue; }

        out.push(sp() + line + ';');
    }
    return out.join('\n');
}

function hPrint(text) { console.log(text); }
const jsCode = transpile(source);
const context = { console: console, process: process, os: os, fs: fs, hPrint: hPrint, commands: commands };
try { vm.createContext(context); vm.runInContext(jsCode, context); } catch (e) { console.error('Runtime error: ' + e.message); }

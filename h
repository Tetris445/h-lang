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
    let indent = 0;
    const sp = () => '    '.repeat(Math.max(0, indent));

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

        // ВЫЗОВ ФУНКЦИИ (без скобок)
        const parts = line.split(/\s+/);
        const isFunc = lines.some(l => l.trim().startsWith('func ' + parts[0] + ' '));
        if (parts.length > 1 && isFunc) {
            const args = parts.slice(1).join(', ');
            out.push(sp() + 'hPrint(' + parts[0] + '(' + args + '));');
            continue;
        }

        // ЦИКЛ FOR
        if (line.startsWith('for ')) {
            const match = line.match(/for\s+(\w+)\s+in\s+(\d+)\s+(\d+)/);
            if (match) { out.push(sp() + 'for (let ' + match[1] + ' = ' + match[2] + '; ' + match[1] + ' <= ' + match[3] + '; ' + match[1] + '++) {'); indent++; continue; }
        }

        // БАЗОВЫЕ КОМАНДЫ (say, red, green и т.д.)
        const firstWord = line.split(/\s+/)[0];
        const rest = line.slice(firstWord.length).trim();
        const isColor = ['red', 'green', 'yellow', 'blue'].includes(firstWord);
        
        if (isColor) {
            out.push(sp() + 'hPrint("\\x1b[' + (firstWord==='red'?'31':firstWord==='green'?'32':firstWord==='yellow'?'33':'34') + 'm" + ' + JSON.stringify(rest) + ' + "\\x1b[0m");');
            continue;
        }
        if (firstWord === 'say') {
            out.push(sp() + 'hPrint(' + JSON.stringify(rest) + ');');
            continue;
        }

        // МАТЕМАТИКА
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('sub ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' - ' + p[1] + ');'); continue; }
        if (line.startsWith('mul ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' * ' + p[1] + ');'); continue; }
        if (line.startsWith('div ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' / ' + p[1] + ');'); continue; }
        if (line.startsWith('sqrt ')) { out.push(sp() + 'hPrint(Math.sqrt(' + line.slice(5) + '));'); continue; }

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
const context = { console: console, process: process, os: os, fs: fs, hPrint: hPrint, commands: commands, Math: Math };
try { vm.createContext(context); vm.runInContext(jsCode, context); } catch (e) { console.error('Runtime error: ' + e.message); }

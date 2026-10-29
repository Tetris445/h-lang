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

        if (line === "version" || line === "ver") { out.push(sp() + 'hPrint("h 2.4.1");'); continue; }

        if (line.startsWith('include ')) {
            const libName = line.slice(8).trim();
            const lib = loadLibrary(libName);
            if (lib) { for (const cmd of Object.keys(lib)) commands[cmd] = lib[cmd]; }
            continue;
        }

        if (line.startsWith('let ') && line.includes('[') && line.includes(']')) {
            const match = line.match(/let\s+(\w+)\s*=\s*\[(.*?)\]/);
            if (match) {
                const name = match[1];
                const values = match[2].split(',').map(v => v.trim()).join(', ');
                out.push(sp() + 'let ' + name + ' = [' + values + '];');
                continue;
            }
        }

        if (line.includes('[') && line.includes(']') && !line.startsWith('let ')) {
            const match = line.match(/(\w+)\[(\d+)\]/);
            if (match) { out.push(sp() + 'hPrint(' + match[1] + '[' + match[2] + ']);'); continue; }
        }

        if (line.startsWith('func ')) {
            const match = line.match(/func\s+(\w+)\s*\((.*?)\)/);
            if (match) { out.push(sp() + 'function ' + match[1] + '(' + match[2] + ') {'); indent++; continue; }
        }
        if (line.startsWith('return ')) { out.push(sp() + 'return ' + line.slice(7) + ';'); continue; }

        if (line.includes('(') && line.includes(')') && !line.startsWith('if ') && !line.startsWith('while ') && !line.startsWith('func ')) {
            out.push(sp() + 'hPrint(' + line + ');'); continue;
        }

        if (line.startsWith('for ')) {
            const match = line.match(/for\s+(\w+)\s+in\s+(\d+)\s+(\d+)/);
            if (match) { out.push(sp() + 'for (let ' + match[1] + ' = ' + match[2] + '; ' + match[1] + ' <= ' + match[3] + '; ' + match[1] + '++) {'); indent++; continue; }
        }

        if (line.startsWith('say ')) { out.push(sp() + 'hPrint(' + JSON.stringify(line.slice(4)) + ');'); continue; }
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('sub ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' - ' + p[1] + ');'); continue; }
        if (line.startsWith('mul ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' * ' + p[1] + ');'); continue; }
        if (line.startsWith('div ')) { const p = line.slice(4).split(/\s+/); out.push(sp() + 'hPrint(' + p[0] + ' / ' + p[1] + ');'); continue; }
        if (line.startsWith('red ')) { out.push(sp() + 'hPrint("\\x1b[31m" + ' + JSON.stringify(line.slice(4)) + ' + "\\x1b[0m");'); continue; }
        if (line.startsWith('green ')) { out.push(sp() + 'hPrint("\\x1b[32m" + ' + JSON.stringify(line.slice(6)) + ' + "\\x1b[0m");'); continue; }

        if (line.startsWith('let ')) { out.push(sp() + line.slice(4) + ';'); continue; }

        if (line.startsWith('if ')) { out.push(sp() + 'if (' + line.slice(3) + ') {'); indent++; continue; }
        if (line === 'else') { if(indent > 0) indent--; out.push(sp() + '} else {'); indent++; continue; }
        if (line.startsWith('while ')) { out.push(sp() + 'while (' + line.slice(6) + ') {'); indent++; continue; }
        if (line === 'end') { if(indent > 0) indent--; out.push(sp() + '}'); continue; }

        const firstWord = line.split(/\s+/)[0];
        if (commands[firstWord]) {
            const rest = line.slice(firstWord.length).trim();
            out.push(sp() + 'hPrint(commands["' + firstWord + '"](' + (isNaN(rest) && rest !== '' ? JSON.stringify(rest) : rest) + '));');
            continue;
        }

        out.push(sp() + line + ';');
    }
    return out.join('\n');
}

function hPrint(text) { console.log(text); }
const jsCode = transpile(source);
const context = { console: console, process: process, os: os, fs: fs, hPrint: hPrint, commands: commands };
try { vm.createContext(context); vm.runInContext(jsCode, context); } catch (e) { console.error('Runtime error: ' + e.message); }

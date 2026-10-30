#!/usr/bin/env node
// Язык h — транспайлер для Node.js
// Версия 2.0 — с поддержкой библиотек

const fs = require('fs');
const vm = require('vm');
const os = require('os');
const path = require('path');

const filename = process.argv[2];
if (!filename) {
    console.error('Usage: h <file.h>');
    process.exit(1);
}
if (!fs.existsSync(filename)) {
    console.error('File not found: ' + filename);
    process.exit(1);
}

const source = fs.readFileSync(filename, 'utf-8');

// ================= БИБЛИОТЕКИ =================
const commands = {};  // реестр команд из библиотек

function loadLibrary(name) {
    const libPath = path.join(__dirname, 'lib', name + '.js');
    if (!fs.existsSync(libPath)) {
        return null;
    }
    delete require.cache[require.resolve(libPath)]; // перезагружаем
    return require(libPath);
}

// ================= ТРАНСПИЛЕР =================
function transpile(code) {
    const lines = code.split('\n');
    const out = [];
    let indent = 0;
    const sp = () => '    '.repeat(indent);

    for (let raw of lines) {
        let line = raw.trim();
        if (!line || line.startsWith('#')) continue;

        // ==== INCLUDE ====
        if (line.startsWith('include ')) {
            const libName = line.slice(8).trim();
            const lib = loadLibrary(libName);
            if (lib) {
                for (const cmd of Object.keys(lib)) {
                    commands[cmd] = lib[cmd];
                }
                out.push(sp() + '// include: ' + libName);
            } else {
                out.push(sp() + 'hPrint("Библиотека не найдена: ' + libName + '");');
            }
            continue;
        }

        // ==== КОМАНДЫ ИЗ БИБЛИОТЕК ====
        const firstWord = line.split(/\s+/)[0];
        if (commands[firstWord]) {
            const rest = line.slice(firstWord.length).trim();
            const args = rest ? rest.split(/\s+/) : [];
            // Числа — без кавычек, остальное — в кавычках
            const formattedArgs = args.map(a => {
                if (!isNaN(a) && a !== '') return a;
                return JSON.stringify(a);
            });
            out.push(sp() + 'hPrint(commands["' + firstWord + '"](' + formattedArgs.join(', ') + '));');
            continue;
        }

        // ==== ВЫВОД ====
        if (line.startsWith('say ')) {
            out.push(sp() + 'hPrint(' + line.slice(4) + ');');
            continue;
        }
        if (line.startsWith('loud ')) {
            out.push(sp() + 'hPrint(String(' + line.slice(5) + ').toUpperCase());');
            continue;
        }
        if (line.startsWith('quiet ')) {
            out.push(sp() + 'hPrint(String(' + line.slice(6) + ').toLowerCase());');
            continue;
        }
        if (line === 'skip') {
            out.push(sp() + 'hPrint("");');
            continue;
        }
        if (line === 'stop') {
            out.push(sp() + 'process.exit(0);');
            continue;
        }

        // ==== АРИФМЕТИКА ====
        if (line.startsWith('add ')) {
            const p = line.slice(4).split(/\s+/);
            out.push(sp() + 'hPrint(' + p[0] + ' + ' + p[1] + ');');
            continue;
        }
        if (line.startsWith('sub ')) {
            const p = line.slice(4).split(/\s+/);
            out.push(sp() + 'hPrint(' + p[0] + ' - ' + p[1] + ');');
            continue;
        }
        if (line.startsWith('mul ')) {
            const p = line.slice(4).split(/\s+/);
            out.push(sp() + 'hPrint(' + p[0] + ' * ' + p[1] + ');');
            continue;
        }
        if (line.startsWith('div ')) {
            const p = line.slice(4).split(/\s+/);
            out.push(sp() + 'hPrint(' + p[0] + ' / ' + p[1] + ');');
            continue;
        }
        if (line.startsWith('random ')) {
            const p = line.slice(7).split(/\s+/);
            out.push(sp() + 'hPrint(Math.floor(Math.random() * (' + p[1] + ' - ' + p[0] + ' + 1)) + ' + p[0] + ');');
            continue;
        }
        if (line.startsWith('sleep ')) {
            const ms = line.slice(6);
            out.push(sp() + 'const _t' + indent + ' = Date.now(); while(Date.now() - _t' + indent + ' < ' + ms + ') {}');
            continue;
        }
        if (line.startsWith('len ')) {
            out.push(sp() + 'hPrint(' + line.slice(4) + '.length);');
            continue;
        }

        // ==== СИСТЕМА ====
        if (line === 'sysinfo') {
            out.push(sp() + 'hPrint("OS: " + os.platform());');
            out.push(sp() + 'hPrint("CPU: " + os.cpus()[0].model);');
            out.push(sp() + 'hPrint("RAM: " + Math.round(os.totalmem() / 1024 / 1024) + " MB");');
            continue;
        }
        if (line.startsWith('exec ')) {
            out.push(sp() + 'try { hPrint(require("child_process").execSync(' + line.slice(5) + ').toString()); } catch(e) { hPrint("Error: " + e.message); }');
            continue;
        }
        if (line.startsWith('read ')) {
            out.push(sp() + 'try { hPrint(fs.readFileSync(' + line.slice(5) + ', "utf-8")); } catch(e) { hPrint("Error: " + e.message); }');
            continue;
        }
        if (line.startsWith('write ')) {
            const rest = line.slice(6);
            const parts = rest.split(' to ');
            if (parts.length === 2) {
                out.push(sp() + 'try { fs.writeFileSync(' + parts[1] + ', ' + parts[0] + '); hPrint("Written"); } catch(e) { hPrint("Error: " + e.message); }');
                continue;
            }
        }

        // ==== ПЕРЕМЕННЫЕ ====
        if (line.startsWith('let ') && line.includes('input()')) {
            const name = line.slice(4).split('=')[0].trim();
            out.push(sp() + 'let ' + name + ' = hRead();');
            continue;
        }
        if (line.startsWith('let ')) {
            let rest = line.slice(4);
            rest = rest.replace(/\bnumber\s+(\w+)/g, 'Number($1)');
            out.push(sp() + 'let ' + rest + ';');
            continue;
        }

        // ==== УСЛОВИЯ ====
        if (line.startsWith('if ')) {
            out.push(sp() + 'if (' + line.slice(3) + ') {');
            indent++;
            continue;
        }
        if (line === 'else') {
            indent--;
            out.push(sp() + '} else {');
            indent++;
            continue;
        }
        if (line.startsWith('while ')) {
            out.push(sp() + 'while (' + line.slice(6) + ') {');
            indent++;
            continue;
        }
        if (line.startsWith('func ')) {
            out.push(sp() + 'function ' + line.slice(5) + ' {');
            indent++;
            continue;
        }
        if (line.startsWith('return ')) {
            out.push(sp() + 'return ' + line.slice(7) + ';');
            continue;
        }
        if (line === 'end') {
            indent--;
            out.push(sp() + '}');
            continue;
        }

        out.push(sp() + line + ';');
    }

    return out.join('\n');
}

function hRead() {
    try {
        const buf = Buffer.alloc(1024);
        const bytes = fs.readSync(0, buf, 0, buf.length, null);
        return buf.toString('utf-8', 0, bytes).trim();
    } catch (e) {
        return '';
    }
}

function hPrint(text) { console.log(text); }

const jsCode = transpile(source);

const context = {
    console: console, process: process, os: os, fs: fs,
    hPrint: hPrint, hRead: hRead,
    commands: commands
};

try {
    vm.createContext(context);
    vm.runInContext(jsCode, context);
} catch (e) {
    console.error('Runtime error: ' + e.message);
} 
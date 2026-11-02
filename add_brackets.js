const fs = require('fs');
let code = fs.readFileSync('h', 'utf8');
const oldStr = "if (line.length === 0 || line.startsWith('#')) continue;";
const newStr = "if (line.length === 0 || line.startsWith('#')) continue;\n        if (line.includes('(') && line.includes(')')) {\n            const inner = line.match(/\\((.*?)\\)/);\n            if (inner) {\n                const innerCmd = inner[1].trim();\n                const outer = line.replace(inner[0], 'RESULT').trim();\n                let innerJs = innerCmd.replace(/add\\\\s+/g, '').replace(/mul\\\\s+/g, '*').replace(/sub\\\\s+/g, '-').replace(/div\\\\s+/g, '/');\n                out.push('var RESULT = ' + innerJs + ';');\n                if (outer.startsWith('add ')) { const p = outer.slice(4).split(/\\\\s+/); out.push('hPrint(' + p[0] + ' + ' + p[1] + ');'); }\n                else if (outer.startsWith('mul ')) { const p = outer.slice(4).split(/\\\\s+/); out.push('hPrint(' + p[0] + ' * ' + p[1] + ');'); }\n                continue;\n            }\n        }\n        ";
code = code.replace(oldStr, newStr);
fs.writeFileSync('h', code);
console.log('✅ Скобки добавлены!');

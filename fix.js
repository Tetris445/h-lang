const fs = require('fs');
let code = fs.readFileSync('h', 'utf8');
const oldStr = "if (!line.startsWith('say') && line.includes('(') && line.includes(')')) {";
const newStr = "if (!line.startsWith('say') && !line.startsWith('mul ') && line.includes('(') && line.includes(')')) {";
code = code.replace(oldStr, newStr);
fs.writeFileSync('h', code);
console.log('✅ Строка исправлена!');

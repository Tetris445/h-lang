const fs = require('fs');
const hCode = fs.readFileSync(process.argv[2], 'utf8');
const lines = hCode.split('\n');
let jsCode = "";
for (let line of lines) {
    let t = line.trim();
    if (!t || t.startsWith("include")) continue;
    if (t.startsWith("say ")) jsCode += `console.log(${t.slice(4).trim()});\n`;
    else if (t.startsWith("let ")) {
        let p = t.slice(4).split("=");
        let name = p[0].trim();
        let val = p[1].trim();
        if (val === "input()") jsCode += `let ${name} = Number(prompt("Ввод:"));\n`;
        else jsCode += `let ${name} = ${val};\n`;
    }
    else if (t.startsWith("add ")) { let a = t.slice(4).split(" "); jsCode += `console.log(${a[0]} + ${a[1]});\n`; }
    else if (t.startsWith("if ")) jsCode += `if (${t.slice(3).trim()}) {\n`;
    else if (t === "end") jsCode += `}\n`;
}
console.log("=== СГЕНЕРИРОВАННЫЙ JS-КОД ===");
console.log(jsCode);

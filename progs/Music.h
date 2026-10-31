<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="utf-8">
<title>h Music</title>
<style>
  body { font-family: monospace; background: #050508; color: #f0f0f5; padding: 20px; }
  h1 { color: #00d4ff; }
  button { padding: 12px 24px; background: #00d4ff; color: #000; border: none; border-radius: 8px; margin: 5px; font-size: 16px; }
  #h-output { background: #0d0d12; padding: 16px; border-radius: 12px; margin-top: 16px; }
</style>
</head>
<body>

<h1>h Music</h1>

<button onclick="playBeep()">Beep 440 Hz</button>
<button onclick="playScale()">Гамма (C-D-E-F-G)</button>

<div id="h-output"></div>

<!-- h-код -->
<script type="text/h">
func note(freq, duration)
    beep freq duration
end
</script>

<!-- JS — звук + связка -->
<script>
  // === Web Audio API — генерация звука ===
  let audioCtx = null;
  
  function hBeep(freq, duration) {
    if (!audioCtx) {
      audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    }
    
    const osc = audioCtx.createOscillator();
    const gain = audioCtx.createGain();
    
    osc.type = 'sine';           // тип волны: sine, square, sawtooth, triangle
    osc.frequency.value = freq;  // частота (Гц)
    
    osc.connect(gain);
    gain.connect(audioCtx.destination);
    
    osc.start();
    gain.gain.setValueAtTime(0.3, audioCtx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, audioCtx.currentTime + duration / 1000);
    osc.stop(audioCtx.currentTime + duration / 1000);
  }
  
  // Кнопка: один beep
  function playBeep() {
    hBeep(440, 500);
    document.getElementById('h-output').innerHTML = '<div>Beep 440 Hz, 500 мс</div>';
  }
  
  // Кнопка: гамма (C-D-E-F-G)
  function playScale() {
    const notes = [262, 294, 330, 349, 392];  // C4, D4, E4, F4, G4
    const out = document.getElementById('h-output');
    out.innerHTML = '';
    
    notes.forEach((freq, i) => {
      setTimeout(() => {
        hBeep(freq, 300);
        const div = document.createElement('div');
        div.textContent = 'Нота ' + (i + 1) + ': ' + freq + ' Гц';
        out.appendChild(div);
      }, i * 350);
    });
  }
</script>

<!-- Транспайлер h → JS -->
<script>
const hCache = new Map();

function hTranspile(code) {
    if (hCache.has(code)) return hCache.get(code);
    const lines = code.split('\n');
    const out = [];
    let indent = 0;
    const space = '    '.repeat(indent);

    for (let raw of lines) {
        let line = raw.trim();
        if (!line || line[0] === '#') continue;

        const sp = '    '.repeat(indent);
        
        if (line.startsWith('say ')) { out.push(sp + 'hPrint(' + line.slice(4) + ');'); continue; }
        if (line.startsWith('beep ')) { const p = line.slice(5).split(/\s+/); out.push(sp + 'hBeep(' + p[0] + ', ' + p[1] + ');'); continue; }
        if (line.startsWith('play ')) { out.push(sp + 'new Audio(' + line.slice(5) + ').play();'); continue; }
        if (line.startsWith('add ')) { const p = line.slice(4).split(/\s+/); out.push(sp + 'hPrint(' + p[0] + ' + ' + p[1] + ');'); continue; }
        if (line.startsWith('let ')) { out.push(sp + 'let ' + line.slice(4) + ';'); continue; }
        if (line.startsWith('func ')) { out.push(sp + 'function ' + line.slice(5) + ' {'); indent++; continue; }
        if (line === 'end') { indent--; out.push(sp + '}'); continue; }
        
        out.push(sp + line + ';');
    }

    const result = out.join('\n');
    hCache.set(code, result);
    return result;
}

function hPrint(text) {
    const out = document.getElementById('h-output');
    const div = document.createElement('div');
    div.textContent = text;
    out.appendChild(div);
}

function runH() {
    const scripts = document.querySelectorAll('script[type="text/h"]');
    for (let s of scripts) {
        const hCode = s.textContent;
        const jsCode = hTranspile(hCode);
        try { eval(jsCode); } catch (e) { console.error(e); }
    }
}

runH();
</script>

</body>
</html>
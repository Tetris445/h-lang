module.exports = {
    // Длина строки
    len: (s) => s.length,
    // Верхний регистр
    upper: (s) => s.toUpperCase(),
    // Нижний регистр
    lower: (s) => s.toLowerCase(),
    // Замена
    replace: (s, a, b) => s.replace(a, b)
};

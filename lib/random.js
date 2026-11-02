module.exports = {
    // Случайное число от 0 до 1
    rand: () => Math.random(),
    // Случайное целое от min до max
    randint: (min, max) => Math.floor(Math.random() * (max - min + 1)) + min
};

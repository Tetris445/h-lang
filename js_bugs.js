console.log("=== ТЕСТ БАГОВ JAVASCRIPT ===");

console.log("1. Сложение:");
console.log("5" + 5);
console.log(5 - "5");

console.log("2. Сравнение:");
console.log(0 == false);
console.log(0 === false);
console.log(null == undefined);
console.log(null === undefined);

console.log("3. Массивы:");
let arr = [1, 2, 3];
arr[10] = 5;
console.log(arr.length);

console.log("4. NaN:");
console.log(10 / "яблоко");
console.log(NaN == NaN);
console.log(isNaN("abc"));

console.log("5. Асинхронность:");
setTimeout(() => console.log("Привет из setTimeout!"), 0);
console.log("А это вывелось раньше!");

console.log("6. Неявное преобразование:");
console.log([] + []);
console.log([] + {});
console.log(true + true);

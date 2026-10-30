say === ТЕСТ ЦЕПОЧКИ ФУНКЦИЙ ===
func add(a, b)
  return a + b
end

func double_add(a, b)
  return add(a, b) + add(a, b)
end

say double_add 2 3

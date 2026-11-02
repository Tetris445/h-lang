let global_var = 100

func check_scope()
  say "Функция видит снаружи:"
  say global_var
end

check_scope

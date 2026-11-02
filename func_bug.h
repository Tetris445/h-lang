func outer()
  say "Снаружи"
  func inner()
    say "Внутри"
  end
  inner
end
outer

let global_var = 100
func check_scope()
  say global_var
end
check_scope

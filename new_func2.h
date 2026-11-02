func outer()
  say "Снаружи"
  func inner()
    say "Внутри"
  end
  inner
end
outer

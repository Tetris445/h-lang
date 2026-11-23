# Changelog

## [v2.5.0] — 2026-11-23

### Added
- **Bash-ОС** (`os_bash.h`) — REPL на чистом Bash (без C и clang).
- **Карективы** (`hprep` + `corecto.h`) — поддержка `say {...}` и `import:{...}`.
- **Библиотеки на чистом h** — `math.h`, `colors.h`, `graphics.h`.
- **VM без libc** (`hvm_asm_nolibc`) — `say`, `let`, `add`, `sub`, `mul`, `div`, `if`, `while`.
- **Указатели** (`P1`–`P5`) в VM.
- **B1/B2** (файлы) в VM.

### Fixed
- Баг с `mul` (умножение) в Bash-ОС.
- Баг с делением на ноль.
- Баг с `A10` в VM.

### Known Limitations
- `LIVE` откачен.
- `T2` не реализован.

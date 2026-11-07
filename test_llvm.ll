; Сгенерировано hllvm из test_llvm.h
target triple = "aarch64-unknown-linux-android"

declare i32 @puts(ptr)
declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %buf = alloca i32
  ; say Hello
  call i32 (ptr, ...) @printf(ptr @.str_09f7e0)
  %x = alloca i32
  store i32 5, ptr %x
  %tmp_add = add i32 %x, %10
  call i32 (ptr, ...) @printf(ptr @.str_int, i32 %tmp_add)
  ret i32 0
}

@.str_09f7e0 = private unnamed_addr constant [7 x i8] c"Hello\0A\00"
@.str_int = private unnamed_addr constant [4 x i8] c"%d\0A\00"

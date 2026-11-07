say "Математика:"
sin 0
cos 0
pow 2 3

say "Файлы:"
file_write test.txt "Привет из файла!"
file_read test.txt

say "Память:"
mem_init 10
mem_alloc arr 3
mem_set arr 0 100
mem_save data.bin
mem_load data.bin
mem_get arr 0

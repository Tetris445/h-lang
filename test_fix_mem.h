mem_init 5
mem_alloc arr 10
mem_set arr 0 777
mem_save arr "data_fixed.bin"

say "Размер файла:"
sys ls -la data_fixed.bin

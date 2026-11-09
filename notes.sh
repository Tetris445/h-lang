#!/bin/bash
echo "Введите заметку:"
read note

echo "mem_init 5" > temp_notes.h
echo "mem_alloc notes 10" >> temp_notes.h
echo "mem_set notes 0 $note" >> temp_notes.h
echo "mem_get notes 0" >> temp_notes.h

echo "Заметка сохранена:"
./h2c temp_notes.h
rm temp_notes.h

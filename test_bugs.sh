#!/bin/bash
# test_bugs.sh — автоматический тест ОС на Bash

# Передаём команды в ОС через pipe
{
    echo "help"
    echo "say Привет, мир!"
    echo "add 5 + 10"
    echo "sub 10 - 5"
    echo "mul 5 * 10"
    echo "div 10 / 2"
    echo "div 10 / 0"
    echo "unknown_command"
    echo ""
    echo "say"
    echo "exit"
} | ./os_bash.h

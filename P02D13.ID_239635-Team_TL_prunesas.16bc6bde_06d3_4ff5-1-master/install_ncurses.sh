#!/bin/bash
# Скрипт установки ncurses для MSYS2 MINGW64

echo "Установка ncurses для MSYS2..."
pacman -S --noconfirm mingw-w64-x86_64-ncurses

echo "Проверка установки..."
if [ -f "/mingw64/include/ncurses.h" ]; then
    echo "✓ ncurses.h найден в /mingw64/include/ncurses.h"
else
    echo "✗ ncurses.h НЕ найден"
fi

echo "Версия GCC:"
gcc --version | head -n 1

echo ""
echo "Готово! Теперь можно компилировать:"
echo "gcc -o game_of_life src/game_of_life.c -lncurses"

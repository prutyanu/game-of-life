# Настройка проекта Game of Life в Windows

## Проблема
VS Code показывает ошибку: `#include errors detected. Please update your includePath.` для библиотеки `<ncurses.h>`

## Решение

### Шаг 1: Установка MSYS2

1. **Скачайте MSYS2:**
   - Перейдите на https://www.msys2.org/
   - Скачайте установщик (msys2-x86_64-YYYYMMDD.exe)

2. **Установите MSYS2:**
   - Запустите установщик
   - Используйте путь по умолчанию: `C:\msys64`
   - Завершите установку

3. **Обновите MSYS2:**
   - Откройте **MSYS2 MSYS** из меню Пуск
   - Выполните команду:
   ```bash
   pacman -Syu
   ```
   - Закройте терминал и откройте снова
   - Повторите:
   ```bash
   pacman -Syu
   ```

### Шаг 2: Установка компилятора и ncurses

1. **Откройте MSYS2 MINGW64** (важно именно MINGW64!)

2. **Установите необходимые пакеты:**
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-ncurses
   pacman -S make
   ```

3. **Проверьте установку:**
   ```bash
   gcc --version
   find /mingw64/include -name ncurses.h
   ```

   Вы должны увидеть путь к ncurses.h, например:
   ```
   /mingw64/include/ncurses/ncurses.h
   ```

### Шаг 3: Добавление MSYS2 в PATH (Windows)

1. **Откройте "Переменные среды":**
   - Нажмите Win + R
   - Введите: `sysdm.cpl`
   - Вкладка "Дополнительно" → "Переменные среды"

2. **Добавьте в PATH:**
   - В "Системные переменные" найдите переменную `Path`
   - Нажмите "Изменить"
   - Добавьте новую запись: `C:\msys64\mingw64\bin`
   - Нажмите OK

3. **Перезапустите VS Code** (полностью закройте и откройте снова)

### Шаг 4: Проверка настроек VS Code

Файл `.vscode/c_cpp_properties.json` уже настроен со следующими путями:

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/msys64/mingw64/include",
                "C:/msys64/mingw64/include/ncurses",
                "C:/msys64/usr/include"
            ],
            "compilerPath": "C:/msys64/mingw64/bin/gcc.exe",
            "cStandard": "c11",
            "intelliSenseMode": "gcc-x64"
        }
    ],
    "version": 4
}
```

**Если у вас MSYS2 установлен в другую директорию:**
- Замените `C:/msys64` на ваш путь
- Используйте прямые слэши `/` даже в Windows

### Шаг 5: Перезагрузка IntelliSense в VS Code

1. Откройте файл `src/game_of_life.c` в VS Code
2. Нажмите `Ctrl + Shift + P`
3. Введите: `C/C++: Restart IntelliSense`
4. Нажмите Enter

Ошибка должна исчезнуть!

---

## Компиляция в Windows

### Вариант 1: Через MSYS2 MINGW64 терминал

1. **Откройте MSYS2 MINGW64** (не MSYS, не UCRT64!)

2. **Перейдите в директорию проекта:**
   ```bash
   cd /c/Users/склад-2/Documents/P02D13.ID_239635-Team_TL_prunesas.16bc6bde_06d3_4ff5-1-master
   ```

3. **Компиляция:**
   ```bash
   gcc -Wall -Wextra -o game_of_life src/game_of_life.c -lncurses
   ```

4. **Запуск:**
   ```bash
   ./game_of_life < initial_states/glider.txt
   ```

### Вариант 2: Через VS Code интегрированный терминал

1. **Настройте терминал по умолчанию:**
   - В VS Code нажмите `Ctrl + Shift + P`
   - Введите: `Terminal: Select Default Profile`
   - Выберите: `MSYS2 MINGW64` (если нет в списке - см. ниже)

2. **Если MSYS2 нет в списке терминалов:**

   Создайте файл `.vscode/settings.json`:
   ```json
   {
       "terminal.integrated.profiles.windows": {
           "MSYS2 MINGW64": {
               "path": "C:\\msys64\\usr\\bin\\bash.exe",
               "args": [
                   "--login",
                   "-i"
               ],
               "env": {
                   "MSYSTEM": "MINGW64",
                   "CHERE_INVOKING": "1"
               }
           }
       },
       "terminal.integrated.defaultProfile.windows": "MSYS2 MINGW64"
   }
   ```

3. **Откройте новый терминал:**
   - `Ctrl + Shift + \``
   - Должен открыться MSYS2 MINGW64

4. **Компиляция и запуск:**
   ```bash
   gcc -Wall -Wextra -o game_of_life src/game_of_life.c -lncurses
   ./game_of_life < initial_states/glider.txt
   ```

### Вариант 3: Создание задач VS Code (tasks.json)

Создайте файл `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Компиляция Game of Life",
            "type": "shell",
            "command": "C:/msys64/mingw64/bin/gcc.exe",
            "args": [
                "-Wall",
                "-Wextra",
                "-o",
                "game_of_life",
                "src/game_of_life.c",
                "-lncurses"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

Теперь можно компилировать через `Ctrl + Shift + B`

---

## Решение типичных проблем в Windows

### Проблема 1: "ncurses.h: No such file or directory"

**Причина:** Не установлен пакет ncurses

**Решение:**
```bash
# В MSYS2 MINGW64
pacman -S mingw-w64-x86_64-ncurses
```

### Проблема 2: "gcc: command not found"

**Причина:** Не установлен компилятор или не добавлен в PATH

**Решение:**
```bash
# В MSYS2 MINGW64
pacman -S mingw-w64-x86_64-gcc

# Проверка:
gcc --version
```

### Проблема 3: VS Code все еще показывает ошибку

**Решения:**

1. **Перезагрузите IntelliSense:**
   - `Ctrl + Shift + P` → `C/C++: Restart IntelliSense`

2. **Проверьте пути в c_cpp_properties.json:**
   - Убедитесь, что используются прямые слэши `/`
   - Пример: `C:/msys64/mingw64/include`

3. **Установите расширение C/C++:**
   - В VS Code: Extensions (Ctrl+Shift+X)
   - Найдите: "C/C++" от Microsoft
   - Установите

4. **Полностью перезапустите VS Code:**
   - Закройте все окна
   - Откройте заново

### Проблема 4: "undefined reference to 'initscr'"

**Причина:** Не подключена библиотека ncurses при компиляции

**Решение:** Обязательно добавьте `-lncurses` в конце команды:
```bash
gcc -o game_of_life src/game_of_life.c -lncurses
```

### Проблема 5: Программа не запускается в обычном CMD или PowerShell

**Причина:** ncurses требует Unix-подобный терминал

**Решение:** Используйте только MSYS2 MINGW64 терминал для запуска программы

### Проблема 6: Странные символы или не работает отображение

**Причина:** Неправильная кодировка терминала

**Решение:**
```bash
# В MSYS2 перед запуском:
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
./game_of_life < initial_states/glider.txt
```

---

## Проверка правильности установки

### Тест 1: Компилятор
```bash
gcc --version
```
Должно вывести версию GCC (например, 13.2.0)

### Тест 2: Библиотека ncurses
```bash
find /mingw64/include -name ncurses.h
```
Должно найти файл ncurses.h

### Тест 3: Компиляция простейшей программы
Создайте файл `test.c`:
```c
#include <ncurses.h>

int main() {
    initscr();
    printw("Hello ncurses!");
    refresh();
    getch();
    endwin();
    return 0;
}
```

Компиляция:
```bash
gcc -o test test.c -lncurses
./test
```

Должно показать "Hello ncurses!" и закрыться после нажатия клавиши.

### Тест 4: VS Code IntelliSense
Откройте `src/game_of_life.c` и наведите курсор на `initscr()` - должна появиться документация функции.

---

## Альтернатива: WSL (Windows Subsystem for Linux)

Если MSYS2 не работает, можно использовать WSL:

1. **Установите WSL:**
   ```powershell
   wsl --install
   ```

2. **В WSL установите зависимости:**
   ```bash
   sudo apt-get update
   sudo apt-get install gcc libncurses5-dev
   ```

3. **В VS Code установите расширение:**
   - "WSL" от Microsoft

4. **Откройте проект в WSL:**
   - `Ctrl + Shift + P` → `WSL: Open Folder in WSL`

---

## Быстрая шпаргалка команд

```bash
# Компиляция
gcc -Wall -Wextra -o game_of_life src/game_of_life.c -lncurses

# Запуск с разными конфигурациями
./game_of_life < initial_states/glider.txt
./game_of_life < initial_states/blinker.txt
./game_of_life < initial_states/toad.txt
./game_of_life < initial_states/beacon.txt
./game_of_life < initial_states/pulsar.txt

# Проверка стиля
clang-format -n -style=Google src/game_of_life.c

# Статический анализ
cppcheck --enable=all src/game_of_life.c
```

---

## Контрольный список

- [ ] MSYS2 установлен в `C:\msys64`
- [ ] Установлен mingw-w64-x86_64-gcc
- [ ] Установлен mingw-w64-x86_64-ncurses
- [ ] `C:\msys64\mingw64\bin` добавлен в PATH
- [ ] VS Code перезапущен после изменений
- [ ] Файл c_cpp_properties.json обновлен
- [ ] IntelliSense перезагружен
- [ ] Расширение C/C++ установлено
- [ ] Тестовая компиляция проходит успешно
- [ ] Программа запускается и работает

---

## Полезные ссылки

- [MSYS2 официальный сайт](https://www.msys2.org/)
- [VS Code C++ документация](https://code.visualstudio.com/docs/cpp/config-mingw)
- [ncurses для Windows](https://packages.msys2.org/package/mingw-w64-x86_64-ncurses)

---

Если после всех шагов проблема остается, проверьте:
1. Правильно ли установлен MSYS2 (именно в `C:\msys64`)
2. Используете ли вы MINGW64, а не обычный MSYS
3. Перезагружен ли VS Code после изменений

Удачи! 🚀

# c_test_task

Интерпретатор скриптового языка для загрузки shared library с поддержкой 2-ух команд: 
use "lib_name.so"
call "function_name"

## Необходимые пакеты
Установить **все** необходимые пакеты
arch
```
pacman -S check valgrind clang-format
```
debian(ubuntu)-like
```
apt install check valgrind clang-format
```

Для проведения тестов установите **check.h**
```
pacman -S check
apt install check
```

Для проведения тестов на утечки памяти установите **valgrind**
```
pacman -S valgrind
apt install valgrind
```

Для работы с форматированием установите **clang-format**
```
pacman -S clang-format
apt install clang-format
```

## Сборка
Сборка исполняемого файла с меню выбора мода
```
make
```

Сборка исполняемого файла только с ручным вводом
```
make manual_mode
```
Сборка исполняемого файла только с автоматическим режимом
```
make manual_mode
```


Запуск исполняемого файла, в случае сборки с ручны вводом
```
make run
```




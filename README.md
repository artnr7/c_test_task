# c_test_task

Интерпретатор скриптового языка для загрузки shared library с поддержкой 2-ух команд: 
use "lib_name.so"
call "function_name"

## Необходимые пакеты
Установить **все** необходимые пакеты
arch
```
pacman -S make check valgrind clang-format
```
debian(ubuntu)-like
```
apt install make check valgrind clang-format
```

Для сборки установите **make**
```
pacman -S make
apt install make
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

## Запуск
Запуск исполняемого файла, в случае сборки с ручны вводом
```
make run
```

## Проверка работоспособности
Для проверки работоспособности можно использовать .so файлы, находящиеся в директории so
Однако, вполне вероятно из-за различий архитектуры понадобится собрать .so файлы вновь с помощью Makefile, располагающийся там же
```
make (all)
``` 

Так же в директории scripts есть несколько примеров скриптов, которые можно использовать в выбранном для интерпретатора авто режиме

## Пример сборки и использования
```sh
make all # choose mode
./interpretator ../scripts/test2.sc
```

```sh
make manual_mode # manual mode
use ../so/car.so
call car_desc
```

```sh
make auto_mode # auto mode
./interpretator ../scripts/test.sc

```

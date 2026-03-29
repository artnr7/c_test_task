# c_test_task

Интерпретатор скриптового языка для загрузки shared library с поддержкой 2-ух команд: 
use "lib_name.so"
call "function_name"

Выход осуществляется по нажатию 'q'

## Необходимые пакеты
Установить **все** необходимые пакеты
arch
```
pacman -S make check valgrind clang-format gcov lcov
```
debian(ubuntu)-like
```
apt install make check valgrind clang-format gcov lcov
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

Для установления процента покрытия тестами установите **gcov** и **lcov**
```
pacman -S gcov lcov
apt install gcov lcov
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
make auto_mode
```

## Запуск
Запуск исполняемого файла, только в случае сборки с ручным вводом
```
make run
```
Иначе нужно запускать exe
```
./src/interpretator
```

## Проверка работоспособности
Для проверки работоспособности можно использовать .so файлы, находящиеся в директории **so**
Однако, вполне вероятно из-за различий архитектуры понадобится собрать .so файлы вновь с помощью Makefile, располагающийся там же
```
make so
``` 

Так же в директории **scripts** есть несколько примеров скриптов, которые можно использовать в выбранном для интерпретатора авто режиме

## Пример сборки и использования
```sh
make all # choose mode
./src/interpretator scripts/test2.sc
```

```sh
make manual_mode # manual mode
make run
use so/car.so
call car_desc
```

```sh
make auto_mode # auto mode
./src/interpretator scripts/test.sc

```
## Тесты
```
make test
```
## Покрытие тестами
```
make gcov_report
```

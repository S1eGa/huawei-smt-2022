# Тестовое задание HUAWEI #

## Зависимости ##

* [cvc5 library](https://github.com/cvc5/cvc5)
* CMake, Make 

## Компиляция ##

Чтобы собрать проект нужно выполнить последовательность команд `cmake .`, затем `make`. Предполагается, что решение будет собираться под ОС Linux.

Решение соберется в файл с названием `solver`.

Для запуска `./solver` может потребоваться добавить директорию с динамической библиотекой `cvc5` (в моем случае это, например, `/usr/local/lib/`) в переменную `$LD_LIBRARY_PATH`.

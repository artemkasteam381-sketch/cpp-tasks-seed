# Домашние задания по дисциплине «Алгоритмы и структуры данных»

## Состояние

- ![BuildUtestItest](https://github.com/dluciv/cpp-tasks-seed/actions/workflows/make_and_test.yml/badge.svg) — Make и тесты
- ![StyleCheck](https://github.com/dluciv/cpp-tasks-seed/actions/workflows/lint_style_check.yml/badge.svg) — CLang Tidy и форматирование

## Задания

### [00.DummyMake](00.DummyMake)

Шаблонное задание с Makefile, модульными и интеграционными тестами.

### [01.InplaceSorting](01.InplaceSorting)

Сортировка на месте.

1. Разобраться, что там уже есть, что как и почему.
2. Реализовать не менее одного алгоритма сортировки за $O(N^2)$ и не менее одного за $O(N log N)$.
3. Добавить их запуск на массивах разой длины в качестве интеграционного теста.
4. Добавить юниттесты для них.

### [02.Base85](02.Base85)

1. Реализовать кодер и декодер.
2. Добавить юниттесты для разной длины разных строк, да попопдлее!

### ... Добавляйте следующие аналогично...

## Что рекомендуется установить и как пользоваться?

### Что установить

1. Разумеется, Git.
2. Linux. С ним легче пойдёт.
3. То, что [в зависимостях](.github/actions/install-prerequisites/action.yml).
4. Visual Studio Code
   - [плагин CodeLLDB](https://open-vsx.org/extension/vadimcn/vscode-lldb)
   - [плагин Astyle](https://open-vsx.org/extension/haloscript/astyle-lsp-vscode)

### Как пользоваться

Просто. Для каждого каталога не забывайте запускать:

- `make all`
- `make astyle`
- `make tidy`

и внимательно смотреть, что произошло.

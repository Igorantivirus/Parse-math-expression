# Parse-math-expression

## Описание
Проект представляет собой парсер строки в математическое выражение с последующим его решением.

## Установка
Для установки скопируйте все файлы `.hpp` из репозитория и подключите их к вашему проекту.

## Пример использования
```cpp
#include <iostream>
#include "MathPars.hpp"

int main()
{
    using namespace expr;

    Parser parser;
    std::string s;

    while (true)
    {
        std::cout << "Введите выражение: ";
        std::getline(std::cin, s);

        std::cout << parser.parse(s).getProcessingResult().getValue() << '\n';
    }

    return 0;
}

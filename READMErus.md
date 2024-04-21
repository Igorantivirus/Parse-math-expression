# Parse-math-expression

## Описание
Проект представляет собой парсер строки в математическое выражение с последующим его решением.

## Установка

Для успешной сборки и использования этого проекта вам потребуется следующее:

Скопируйте все файлы `.hpp` из репозитория и подключите их к вашему проекту.

## Пример использования
```cpp
#include<iostream>

#include<MathParse/MathPars.hpp>

int main()
{
	using namespace expr;
	using namespace std;

	Parser parser;
	std::string s;

	while (true)
	{
		std::cout << "Enter: ";
		std::getline(std::cin, s);

		try
		{
			std::cout << parser.parse(s).getProcessingResult().getValue() << '\n';
		}
		catch (const expr::ParseException& e)
		{
			std::cout << e.what() << '\n';
		}
	}

	return 0;
}
```

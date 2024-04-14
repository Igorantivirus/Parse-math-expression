# Parse-math-expression

## Описание
Проект представляет собой парсер строки в математическое выражение с последующим его решением.

## Установка

Для успешной сборки и использования этого проекта вам потребуется следующее:

1. Скопируйте все файлы `.hpp` из репозитория и подключите их к вашему проекту.
2. Убедитесь, что у вас установлена библиотека Boost. Если нет, вы можете скачать ее с [официального сайта](https://www.boost.org/).
3. Укажите путь к заголовочным файлам Boost в вашем проекте или воспользуйтесь инструкциями по установке библиотеки для вашей среды разработки.

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

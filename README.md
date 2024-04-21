# Parse-math-expression

## Description
The project is a parser for a string into a mathematical expression followed by its solution.

## Installation

To successfully build and use this project, you will need the following:

Copy all `.hpp` files from the repository and include them in your project.

## Example Usage
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

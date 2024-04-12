# Parse-math-expression

## Description
The project is a parser for a string into a mathematical expression followed by its solution.

## Installation
To install, copy all `.hpp` files from the repository and include them in your project.

## Example Usage
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
        std::cout << "Enter expression: ";
        std::getline(std::cin, s);

        std::cout << parser.parse(s).getProcessingResult().getValue() << '\n';
    }

    return 0;
}

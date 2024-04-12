# About the Project

The project contains a set of 3 .hpp files:

1. **Utility functions and enum enumerations**
2. **A set of classes for processing mathematical expressions:**
    - **Value**: numeric value
    - **Function**: function or mathematical operator acting as a function
    - **Expression**: a set of elements of all three types listed above, for their processing, can be used as an expression in parentheses
3. **Class for parsing a string into a mathematical expression:**
    - There is only one public method `parse(std::string)`, which processes the string

# Possible mathematical operators and functions

## Standard operators
1. `+`
2. `-`
3. `*`
4. `/`
5. `^` - exponentiation

## Programming operators
1. `//` - integer division
2. `%` - remainder of division

## Additional features
1. `log(a)(b)` - logarithm of number `b` to the base `a` (without spaces)
2. `!` - factorial of number (numbers Z >= 0)
3. `(a)nrt(b)` - root of number `b` to the power of `a`
4. **Constants**
    - `pi`: 3.1415926535897932
    - `e`: 2.718281828459045
5. `(a)d` - convert number `a` from degrees to radian
6. `(a)r` - convert number `a` from radian to degrees
7. `i` - imaginary unit `i=sqrt(-1)`

## Functions
1. **Trigonometry**
    - `sin`
    - `cos`
    - `tg`
    - `ctg`
    - `arcsin`
    - `arccos`
    - `arctg`
    - `arcctg`
2. **Algebra**
    - `sqrt`
    - `ln`
    - `lg`
    
## Brackets
1. `()` - ordinary
2. `[]` - integer part of the number
3. `{}` - fractional part of the number
4. `||` - module

#pragma once

#include<string>
#include<iostream>
#include<complex>

#define PI_val 3.1415926535897932l

namespace expr
{
	using FType = std::complex<long double>;
	//using FType = long double;

	enum class Action : char
	{
		none = 0,		//
		plus = '+',		// +
		minus = '-',	// -
		div = '/',		// -
		rdiv = '%',		// %
		mult = '*',		// *
		pow = '^',		// ^
		log,			// log
		nrt,			// nrt
		idiv			// //
	};
	enum class FunctionType : char
	{
		none,	//
		sqrt,	//sqrt(x)
		ln,		//log(x)
		lg,		//log10(x)
		sin,	//sin(x)
		cos,	//cos(x)
		tg,		//tan(x)
		ctg,	//1/tan(x)
		arcsin,	//asin(x)
		arccos,	//acos(x)
		arctg,	//atan(x)
		arcctg,	//pi/2-atan(x)
		fact,	//tgamma(x+1)
		degrees,//convert degrees to radian
		radian,	//convert radian to degrees
	};
	enum class Brackets : char
	{
		none,
		def,	//default	()
		round,	//round		[]
		frac,	//fraction	{}
		modul	//modul		||
	};

	namespace enumFunc
	{
		Action parseAction(const std::string& s)
		{
			switch (s[0])
			{
			case '+':
				return Action::plus;
			case '-':
				return Action::minus;
			case '*':
				return Action::mult;
			case '%':
				return Action::rdiv;
			case '^':
				return Action::pow;
			default:
				if (s == "//")
					return Action::idiv;
				if (s == "/")
					return Action::div;
				if (s == "nrt")
					return Action::nrt;
				return Action::none;
			}
		}
		Brackets parseBracket(const char c)
		{
			switch (c)
			{
			case '(':
			case ')':
				return Brackets::def;
			case '[':
			case ']':
				return Brackets::round;
			case '{':
			case '}':
				return Brackets::frac;
			case '<':
			case '>':
				return Brackets::modul;
			default:
				return Brackets::none;
			}
		}
		FunctionType parseFunction(const std::string& s)
		{
			if (s == "sqrt")
				return FunctionType::sqrt;
			if (s == "ln")
				return FunctionType::ln;
			if (s == "lg")
				return FunctionType::lg;
			if (s == "sin")
				return FunctionType::sin;
			if (s == "cos")
				return FunctionType::cos;
			if (s == "tg")
				return FunctionType::tg;
			if (s == "ctg")
				return FunctionType::ctg;
			if (s == "arcsin")
				return FunctionType::arcsin;
			if (s == "arccos")
				return FunctionType::arccos;
			if (s == "arctg")
				return FunctionType::arctg;
			if (s == "arcctg")
				return FunctionType::arcctg;
			if (s == "fact")
				return FunctionType::fact;
			if (s == "d")
				return FunctionType::degrees;
			return FunctionType::none;
		}
		FunctionType parseSpecialType(const char c)
		{
			if (c == '!')
				return FunctionType::fact;
			if (c == 'd')
				return FunctionType::degrees;
			if (c == 'r')
				return FunctionType::radian;
			return FunctionType::none;
		}

		bool specialFunc(const char c)
		{
			return c == 'd' || c == '!' || c == 'r';
		}
		bool isOpenBracket(const char c)
		{
			return c == '(' || c == '{' || c == '[' || c == '<';
		}
		bool isCloseBracket(const char c)
		{
			return c == ')' || c == '}' || c == ']' || c == '>';
		}
		bool isNum(const std::string& str)
		{
			return str[0] >= '0' && str[0] <= '9';
		}
		bool isWord(const std::string& str)
		{
			return str[0] >= 'a' && str[0] <= 'z' || str[0] >= 'A' && str[0] <= 'Z';
		}
		char oppositeBracket(const char c)
	{
		switch (c)
		{
		case '(': return ')';
		case ')': return '(';
		case '[': return ']';
		case ']': return '[';
		case '{': return '}';
		case '}': return '{';
		case '<': return '>';
		case '>': return '<';
		default: return '\0';
		}
	}

		bool firstAction(const Action a)
	{
		return a == Action::pow || a == Action::log || a == Action::nrt;
	}
		bool secondAction(const Action a)
	{
		return a == Action::mult || a == Action::div || a == Action::rdiv || a == Action::idiv;
	}
		bool thirdAction(const Action a)
	{
		return a == Action::plus || a == Action::minus;
	}

		void outAct(const Action a)
		{
			switch (a)
			{
			case Action::log:
				std::cout << "log";
				break;
			case Action::nrt:
				std::cout << "nrt";
				break;
			case Action::idiv:
				std::cout << "//";
				break;
			default:
				std::cout << static_cast<char>(a);
				break;
			}
		}
		void outFunc(const FunctionType a)
		{
			switch (a)
			{
			case FunctionType::sqrt:
				std::cout << "sqrt";
				break;
			case FunctionType::ln:
				std::cout << "ln";
				break;
			case FunctionType::lg:
				std::cout << "lg";
				break;
			case FunctionType::sin:
				std::cout << "sin";
				break;
			case FunctionType::cos:
				std::cout << "cos";
				break;
			case FunctionType::tg:
				std::cout << "tg";
				break;
			case FunctionType::ctg:
				std::cout << "ctg";
				break;
			case FunctionType::arcsin:
				std::cout << "arcsin";
				break;
			case FunctionType::arccos:
				std::cout << "arccos";
				break;
			case FunctionType::arctg:
				std::cout << "arctg";
				break;
			case FunctionType::arcctg:
				std::cout << "arcctg";
				break;
			case FunctionType::fact:
				std::cout << "!";
				break;
			default:
				break;
			}
		}
		void outOpenBrack(const Brackets a)
		{
			switch (a)
			{
			case Brackets::def:
				std::cout << "(";
				break;
			case Brackets::round:
				std::cout << "[";
				break;
			case Brackets::frac:
				std::cout << "{";
				break;
			case Brackets::modul:
				std::cout << "|";
				break;
			default:
				break;
			}
		}
		void outCloseBrack(const Brackets a)
	{
		switch (a)
		{
		case Brackets::def:
			std::cout << ")";
			break;
		case Brackets::round:
			std::cout << "]";
			break;
		case Brackets::frac:
			std::cout << "}";
			break;
		case Brackets::modul:
			std::cout << "|";
			break;
		default:
			break;
		}
	}
	}

	namespace myMath
	{
		FType toRadian(const FType& g)
		{
			return g * FType(PI_val / 180.l);
		}
		FType toDegrees(const FType& g)
		{
			return g * FType(180.l / PI_val);
		}
		FType toFType(const std::string& str)
		{
			auto pr = std::stold(str);
			if (str.back() == 'i')
				return std::complex<double>(0, pr);
			return std::complex<double>(pr, 0);
		}
		FType floor(FType v)
		{
			v.real(std::floor(v.real()));
			v.imag(std::floor(v.imag()));
			return v;
		}
		FType frac(FType v)
		{
			return v - floor(v);
		}
		FType fmod(FType x, FType y)
		{
			FType quotient = x / y;
			auto a = quotient.real();
			auto b = quotient.imag();
			std::modf(a, &a);
			std::modf(b, &b);
			FType integer_part(a, b);
			return x - integer_part * y;
		}
		FType factorial(const FType& a)
	{
		return std::tgamma(a.real() + 1);
	}
	}

	void replaceAll(std::string& str, const char* oldS, const char* newS)
	{
		const size_t s1 = std::strlen(oldS);
		const size_t s2 = std::strlen(newS);
		size_t ind;
		size_t last = 0;
		while ((ind = str.find(oldS, last)) != std::string::npos)
		{
			str.replace(ind, s1, newS);
			last = ind + s2;
		}
	}

}

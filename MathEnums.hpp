#pragma once

#include<string>
#include<iostream>
#include<complex>

#define PI_val 3.1415926535897932l

namespace expr
{
	//using FType = std::complex<long double>;
	using FType = long double;

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

	FType toRadian(const FType& g)
	{
		return g * FType(PI_val / 180.l);
	}
	FType toDegrees(const FType& g)
	{
		return g * FType(180.l / PI_val);
	}

	FType myFloor(const FType& v)
	{
		return std::floor(v);
	}
	FType myFrac(FType v)
	{
		return v - myFloor(v);
	}
	FType myFmod(FType x, FType y)
	{
		return std::fmod(x, y);
	}

	FType toFType(const std::string& str)
	{
		return std::stold(str);
	}

	FType myFactorial(const FType& a)
	{
		return std::tgamma(a + 1);
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

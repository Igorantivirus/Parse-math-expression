#pragma once

#include<string>
#include<algorithm>

#include"Fraction.hpp"

namespace expr
{
	template<typename Complex>
	class Variable
	{
	public:

		Variable() = default;
		Variable(const char var, const frac::Fraction& degr = 1) : _var(var), _degr(degr) {}

		const char getVar() const
		{
			return _var;
		}
		const frac::Fraction& getDegr() const
		{
			return _degr;
		}

		void setVar(const char var)
		{
			if(var >= 'A' && var <= 'Z' || var >= 'a' && var <= 'z')
				_var = var;
		}
		void setDegr(const frac::Fraction& degr)
		{
			_degr = degr;
		}

		const std::string toString() const
		{
			return std::string(1, _var) + (_degr == 1 ? "" : "^(" + _degr.toString(true)) + ')';
		}
		const std::string toMathJaxString() const
		{
			std::string res(1, _var);
			if (_degr == 1)
				return res;
			if (_degr.denominator() == 1)
				return res + "^{" + PolinomUtils::toString(_degr.numerator()) + '}';
			return "\\sqrt[" + PolinomUtils::toString(_degr.numerator()) + "]{" + res + "^" + PolinomUtils::toString(_degr.numerator()) + '}';
		}

		bool operator<(const Variable& other) const
		{
			return _var < other._var;
		}
		bool operator==(const Variable& other) const
		{
			return _var == other._var && _degr == other._degr;
		}
		bool operator!=(const Variable& other) const
		{
			return !this->operator==(other);
		}

	private:

		char _var = 'x';
		frac::Fraction _degr = 1;

	};
}
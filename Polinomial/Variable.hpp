#pragma once

#include<string>
#include<algorithm>

//#include"Fraction.hpp"
#include"../MathWorker/MathWorker.hpp"

namespace expr
{
	template<typename Complex>
	class Variable
	{
	public:

		Variable() = default;
		Variable(const char var, const Complex& degr = 1) : _var(var), _degr(degr) {}

		const char getVar() const
		{
			return _var;
		}
		const Complex& getDegr() const
		{
			return _degr;
		}

		void setVar(const char var)
		{
			if(var >= 'A' && var <= 'Z' || var >= 'a' && var <= 'z')
				_var = var;
		}
		void setDegr(const Complex& degr)
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
			if (_degr == Complex(1))
				return res;
			mathWorker::MathWorker<Complex> wrkr;
			return res + "^{" + wrkr.toMathJaxStr(_degr) + '}';
		}

		bool operator<(const Variable& other) const
		{
			return _var < other._var;
		}
		bool operator==(const Variable& other) const
		{
			return (_var == other._var) && (_degr == other._degr);
		}
		bool operator!=(const Variable& other) const
		{
			return !this->operator==(other);
		}

	private:

		char _var = 'x';
		Complex _degr = 1;

	};
}
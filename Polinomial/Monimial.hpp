#pragma once

#include<string>
#include<algorithm>
#include<vector>

//#include"PolinomUtils.hpp"
#include"Variable.hpp"

#include"../MathWorker/MathWorker.hpp"

namespace expr
{
	template<typename Complex>
	class Monomial
	{
	public:

		#pragma region Constructors

		Monomial() = default;
		Monomial(const Complex& num, const std::vector<Variable<Complex>>& coefs) :
			_num(num)
		{
			setCoefs(coefs);
		}
		Monomial(const Complex& num, const char var, const Complex degr = 1) :
			_num(num)
		{
			setCoefs(var, degr);
		}
		explicit Monomial(const Complex& num) :
			_num(num)
		{}
		template<std::integral T>
		Monomial(T num) :
			_num(Complex(num))
		{}
		explicit Monomial(const char var) :
			_num(1)
		{
			setCoefs(var, 1LL);
		}

		#pragma endregion

		#pragma region Methods

		const std::vector<Variable<Complex>>& getCoefs() const
		{
			return _coefs;
		}
		const Complex& getNum() const
		{
			return _num;
		}

		void setCoefs(const std::vector<Variable<Complex>>& coefs)
		{
			_coefs = coefs;
			sortCoefs();
		}
		void setCoefs(const char coef, const Complex& degr)
		{
			Variable<Complex> var = { coef, degr };
			_coefs.clear();
			_coefs.push_back(var);
		}
		void setNum(const Complex& num)
		{
			_num = num;
			if (_num == Complex(0))
				_coefs.clear();
		}

		const std::string toString() const
		{
			mathWorker::MathWorker<Complex> wrk;
			std::string res = wrk.toStr(_num);
			for (const auto& i : _coefs)
				res += i.toString();
			return res;
		}
		const std::string toMathJaxString() const
		{
			std::string num;
			std::string denom;
			for (const auto& i : _coefs)
			{
				if (i.getDegr() > 0)
					num += i.toMathJaxString();
				else
				{
					Variable pr = i;
					pr.setDegr(pr.getDegr() * -1);
					denom += pr.toMathJaxString();
				}
			}
			std::string frac = denom.empty() ? num : ("\\frac{" + (num.empty() ? "1" : num) + "}{" + denom + "}");
			mathWorker::MathWorker<Complex> wrk;
			
			std::string number = wrk.toMathJaxStr(_num);;
			if (std::min(number.find('+'), number.find('-')) != std::string::npos)
				return '(' + number + ')' + frac;

			if (frac.empty())
				return number;
			if (number == "1" || number == "-1")
				return (number == "-1" ? "-" : "") + frac;
			return number + frac;
		}

		#pragma endregion

		#pragma region Operators

		Monomial& operator*=(const Monomial& other)
		{
			_num *= other._num;
			if (this == &other)
			{
				for (auto& i : _coefs)
					i.setDegr(i.getDegr() + 1);
			}
			else
			{
				for (const auto& j : other._coefs)
				{
					auto it = std::find_if(_coefs.begin(), _coefs.end(), [&](const Variable& i)
						{
							return i.getVar() == j.getVar();
						});
					if (it != _coefs.end())
						it->setDegr(it->getDegr() + j.getDegr());
					else
						_coefs.push_back(j);
				}
			}
			sortCoefs();
			return *this;
		}
		Monomial& operator/=(const Monomial& other)
		{
			_num /= other._num;
			if (this == &other)
			{
				for (auto& i : _coefs)
					i.setDegr(i.getDegr() - 1);
			}
			else
			{
				for (const auto& j : other._coefs)
				{
					auto it = std::find_if(_coefs.begin(), _coefs.end(), [&](const Variable& i)
						{
							return i.getVar() == j.getVar();
						});
					if (it != _coefs.end())
						it->setDegr(it->getDegr() - j.getDegr());
					else
					{
						Variable var = j;
						var.setDegr(var.getDegr() * -1);
						_coefs.push_back(var);
					}
				}
			}
			sortCoefs();
			return *this;
		}

		bool operator+=(const Monomial& other)
		{
			if (this == &other)
			{
				_num += other._num;
				return true;
			}
			if (_coefs.size() != other._coefs.size())
				return false;
			for (size_t i = 0; i < _coefs.size(); ++i)
				if (_coefs[i].getVar() != other._coefs[i].getVar() || _coefs[i].getDegr() != other._coefs[i].getDegr())
					return false;
			_num += other._num;
			return true;
		}
		bool operator-=(const Monomial& other)
		{
			if (this == &other)
			{
				_num = 0;
				_coefs.clear();
				return true;
			}
			if (_coefs.size() != other._coefs.size())
				return false;
			for (size_t i = 0; i < _coefs.size(); ++i)
				if (_coefs[i].getVar() != other._coefs[i].getVar())
					return false;
			_num -= other._num;
			return true;
		}

		Monomial operator-() const
		{
			Monomial res = *this;
			res._num *= -1;
			return res;
		}

		bool operator==(const Monomial& other) const
		{
			if (_num != other._num || _coefs.size() != other._coefs.size())
				return false;
			for (size_t i = 0; i < _coefs.size(); ++i)
				if (_coefs[i] != other._coefs[i])
					return false;
			return true;
		}
		bool operator!=(const Monomial& other) const
		{
			return !this->operator==(other);
		}

		#pragma endregion

	private:

		std::vector<Variable<Complex>> _coefs;

		Complex _num = 0;

	private:

		void sortCoefs()
		{
			std::vector<Variable<Complex>> pr = _coefs;
			_coefs.clear();
			for (auto& i : pr)
			{
				bool was = false;
				for (auto& j : _coefs)
					if (j.getVar() == i.getVar())
					{
						//Если переменная уже есть, то объединяем степени
						j.setDegr(j.getDegr() + i.getDegr());
						was = true;
						break;
					}
				if (!was)
					_coefs.push_back(i);
			}
			for (size_t i = _coefs.size(); i > 0; --i)
				if (_coefs[i - 1].getDegr() == 0)
					_coefs.erase(_coefs.begin() + i - 1);
			std::sort(_coefs.begin(), _coefs.end());
		}

	};

}
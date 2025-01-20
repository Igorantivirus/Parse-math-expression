#pragma once

#include<string>
#include<algorithm>
#include<vector>
#include<map>

//#include"PolinomUtils.hpp"
#include"Monimial.hpp"

namespace expr
{
	template<typename Complex>
	class Polinomial
	{
	public:
		Polinomial() = default;
		Polinomial(const Monomial<Complex>& mono) :
			_expression(std::vector<Monomial<Complex>>(1, mono))
		{}

		#pragma region Methods

		const size_t size() const
		{
			return _expression.size();
		}

		const auto begin() const
		{
			return _expression.begin();
		}
		const auto end() const
		{
			return _expression.end();
		}

		auto begin()
		{
			return _expression.begin();
		}
		auto end()
		{
			return _expression.end();
		}

		const std::string toString() const
		{
			std::string res;
			for (const auto& i : _expression)
				res += '+' + i.toString();
			return res;
		}
		const std::string toMathJaxString() const
		{
			std::string res;
			if (!_expression.empty())
				res += _expression[0].toMathJaxString();
			for (size_t i = 1; i < _expression.size(); ++i)
				res += '+' + _expression[i].toMathJaxString();
			return res;
		}

		bool empty() const
		{
			return _expression.empty();
		}

		std::vector<Polinomial> groupAtVar(const Variable<Complex>& var) const
		{
			std::map<Complex, Polinomial, ComplexCompare> map;
			for (const auto& term : _expression)
			{
				Complex degr = 0;
				for (const auto& coef : term.getCoefs())
				{
					if (coef.getVar() == var.getVar())
					{
						degr = coef.getDegr();
						break;
					}
				}
				map[degr] += term;
			}
			std::vector<Polinomial> res;
			res.reserve(map.size());
			for (auto& [key, value] : map)
				res.push_back(std::move(value));
			return res;
		}

		#pragma endregion

		#pragma region operators

		Polinomial operator-() const
		{
			Polinomial res = *this;
			for (auto& i : res)
				i = -i;
			return res;
		}

		Polinomial& operator+=(const Polinomial& other)
		{
			if (this == &other)
			{
				for (auto& i : _expression)
					i.setNum(i.getNum() * Complex(2));
				return *this;
			}
			for (auto& j : other._expression)
			{
				bool was = false;
				for(auto& i : _expression)
					if (i += j)
					{
						was = true;
						break;
					}
				if(!was)
					_expression.push_back(j);
			}
			reduction();
			return *this;
		}
		Polinomial& operator-=(const Polinomial& other)
		{
			if (this == &other)
			{
				_expression.clear();
				return *this;
			}
			for (auto& j : other._expression)
			{
				bool was = false;
				for (auto& i : _expression)
					if (i -= j)
					{
						was = true;
						break;
					}
				if (!was)
					_expression.push_back(-j);
			}
			reduction();
			return *this;
		}
		Polinomial& operator*=(const Polinomial& other)
		{
			std::vector<Monomial> res;
			for(const auto& i : _expression)
				for (const auto& j : other._expression)
				{
					Monomial pr = i;
					pr *= j;
					res.push_back(pr);
				}
			_expression = std::move(res);
			reduction();
			return *this;
		}

		Polinomial operator+(const Polinomial& other) const
		{
			Polinomial pol = *this;
			pol += other;
			return pol;
		}
		Polinomial operator-(const Polinomial& other) const
		{
			Polinomial pol = *this;
			pol -= other;
			return pol;
		}
		Polinomial operator*(const Polinomial& other) const
		{
			Polinomial pol = *this;
			pol *= other;
			return pol;
		}

		const Monomial<Complex>& operator[](const size_t ind) const
		{
			return _expression[ind];
		}
		Monomial<Complex>& operator[](const size_t ind)
		{
			return _expression[ind];
		}

		bool operator==(const Polinomial& other) const
		{
			if (_expression.size() != other._expression.size())
				return false;
			for (size_t i = 0; i < _expression.size(); ++i)
				if (_expression[i] != other._expression[i])
					return false;
			return true;
		}
		bool operator!=(const Polinomial& other) const
		{
			return !this->operator==(other);
		}

		#pragma endregion

		static Polinomial pow(const Polinomial& pol, unsigned int degr)
		{
			Polinomial res = Monomial<Complex>{ Complex(1) };
			for (unsigned int i = 0; i < degr; ++i)
				res *= pol;
			return res;
		}

	private:

		std::vector<Monomial<Complex>> _expression;

	private:

		bool isNull(const Complex& a) const
		{
			return std::abs(a.real()) < 1e-10 && std::abs(a.imag()) < 1e-10;
		}

		void reduction()
		{
			for (size_t i = _expression.size(); i > 0; --i)
				if (isNull(_expression[i - 1].getNum()))
					_expression.erase(_expression.begin() + i - 1);
			for (size_t i = 0; i < _expression.size(); ++i)
				for (size_t j = i + 1; j < _expression.size(); ++j)
					if (_expression[i] += _expression[j])
						_expression.erase(_expression.begin() + j--);
		}

	private:

		struct ComplexCompare
		{
			bool operator()(const Complex& lhs, const Complex& rhs) const
			{
				if (lhs.real() != rhs.real())
					return lhs.real() < rhs.real();
				return lhs.imag() < rhs.imag();
			}
		};

	};

}

template<typename Complex>
expr::Polinomial<Complex> operator+(const expr::Monomial<Complex>& a, const expr::Monomial<Complex>& b)
{
	return expr::Polinomial<Complex>(a) + b;
}
template<typename Complex>
expr::Polinomial<Complex> operator-(const expr::Monomial<Complex>& a, const expr::Monomial<Complex>& b)
{
	return expr::Polinomial<Complex>(a) - b;
}
template<typename Complex>
expr::Polinomial<Complex> operator*(const expr::Monomial<Complex>& a, const expr::Monomial<Complex>& b)
{
	expr::Monomial<Complex> pr = a;
	pr *= b;
	return expr::Polinomial<Complex>(pr);
}
template<typename Complex>
expr::Polinomial<Complex> operator/(const expr::Monomial<Complex>& a, const expr::Monomial<Complex>& b)
{
	expr::Monomial<Complex> pr = a;
	pr /= b;
	return expr::Polinomial<Complex>(pr);
}
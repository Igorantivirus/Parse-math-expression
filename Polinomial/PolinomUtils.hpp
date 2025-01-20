#pragma once

#include<string>
#include<fstream>
#include<complex>
#include<numeric>

#include"Fraction.hpp"

//using NumT = frac::Fraction;
using NumT = std::complex<long double>;

namespace expr
{

	namespace PolinomUtils
	{
		template<typename Complex>
		class StringComplexConverter
		{
		public:
			std::string toStr(const Complex& a) const
			{
				std::string real = toStr(a.real());
				std::string imag = toStr(a.imag());
				if (imag == "0")
					return real;
				else if (real == "0")
					return imag + 'i';
				if (a.imag() > 0)
					return real + '+' + imag + 'i';
				return real + imag + 'i';
			}
			//real type to stirng
			std::string toStr(const decltype(Complex().real()) a) const
			{
				std::string res = std::to_string(a);
				if (res.find('.') != std::string::npos)
				{
					while (res.back() == '0')
						res.pop_back();
					if (res.back() == '.')
						res.pop_back();
				}
				return res == "-0" ? "0" : res;
			}
			Complex toComplex(std::string str) const
			{
				if (str.empty() || str.size() > 300)
					return Complex(nan(""), nan(""));
				if (size_t ind = std::min(str.find('+', 1), str.find('-', 1)); ind != std::string::npos)
				{
					std::string real = str.substr(0, ind);
					std::string imag = str.substr(ind);
					imag.pop_back();
					if (imag == "-" || imag == "+")
						return Complex(noExceptStold(real), (imag[0] == '+') ? 1 : -1);
					return Complex(noExceptStold(real), noExceptStold(imag));
				}
				else if (str.back() == 'i')
				{
					str.pop_back();
					if (str == "-" || str.empty())
						return Complex(0, str.empty() ? 1 : -1);
					return Complex(0, noExceptStold(str));
				}
				else
					return Complex(noExceptStold(str), 0);
			}
		private:

			decltype(Complex().real()) noExceptStold(const std::string& str) const noexcept
			{
				if (str.empty())
					return nan("");
				try
				{
					if (str[0] == '+')
						return stold(str.substr(1));
					return stold(str);
				}
				catch (...)
				{
					return nan("");
				}
			}

		};

		// Функция для проверки, насколько a близко к рациональному числу p/q
		bool isApproxRational(const long double a, long long& p, long long& q, const unsigned maxDenominator = 10000, const long double epsilon = 1e-9)
		{
			long long p0 = 0, p1 = 1; // Числители
			long long q0 = 1, q1 = 0; // Знаменатели

			long long integerPart = 0;
			p = 1;
			q = 1;

			long double val = std::abs(a);
			long double fraction = val;
			while (true)
			{
				integerPart = static_cast<long long>(fraction);
				p = integerPart * p1 + p0;
				q = integerPart * q1 + q0;

				if (q > maxDenominator)
					break; // Знаменатель превышает лимит

				p0 = p1; p1 = p;
				q0 = q1; q1 = q;

				if (std::abs(val - static_cast<long double>(p) / q) < epsilon)
				{
					long long gcd = std::gcd(p, q);
					p /= gcd;
					q /= gcd;
					p *= a < 0 ? -1 : 1;
					return true; // Число близко к рациональному
				}

				fraction = 1 / (fraction - integerPart);
			}
			return false; // Число далеко от рациональных в заданных ограничениях
		}

		std::string toString(const NumT& v)
		{
			StringComplexConverter<NumT> conv;
			return conv.toStr(v);
		}
		std::string toMathJaxStringLLD(const long double v)
		{
			long long p{}, q{};
			if (!isApproxRational(v, p, q) || q == 1)
			{
				StringComplexConverter<NumT> conv;
				return conv.toStr(v);
			}
			return "\\frac{" + std::to_string(p) + "}{" + std::to_string(q) + "}";
		}
		std::string toMathJaxString(const NumT& v)
		{
			std::string num = toMathJaxStringLLD(v.real()), denom = toMathJaxStringLLD(v.imag());
			if (denom == "0")
				return num;
			denom = denom == "1" ? "i" : (denom == "-1" ? "-i" : (denom[0] == '-' ? (denom + 'i') : ('+' + denom + 'i')));
			if (num == "0")
				return denom;
			return num + denom;
		}

		bool isNull(const NumT& a)
		{
			auto null = [](const long double val)->bool
				{
					return std::abs(val) < 1e-10;
				};

			return null(a.real()) && null(a.imag());
		}

		void makeHTML(const std::string& str, const std::string& fName)
		{
			std::ofstream out(fName);

			out <<
	R"(<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Document</title>
		<script src="https://polyfill.io/v3/polyfill.min.js?features=es6"></script>
		<script src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js" async></script>
	</head>
	<body>
	)";
			out << str;
			out <<
	R"(
	</body>
	</html>)";
			out.close();
		}

	


	}

}
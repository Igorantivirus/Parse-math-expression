#pragma once

#include<string>
#include<complex>
#include<numeric>
#include<math.h>

namespace mathWorker
{
	template<typename Complex>
	class MathWorker
	{
	public:
		Complex getPi() const 
		{
			return Complex(std::acos(-1));
		}
		Complex getE() const 
		{
			return Complex(std::exp(1));
		}
		Complex getI() const
		{
			return Complex(0, 1);
		}

		Complex getNan() const 
		{
			return Complex(0) / Complex(0);
		}
		Complex getInf() const 
		{
			return Complex(1) / Complex(0);
		}

		std::string toStr(const Complex& a) const 
		{
			std::string real = toStr(a.real());
			std::string imag = toStr(a.imag());
			if(imag == "0")
				return real;
			else if(real == "0")
				return imag + 'i';
			if(a.imag() > 0)
				return real + '+' + imag + 'i';
			return real + imag + 'i';
		}
		std::string toMathJaxStr(const Complex& a) const
		{
			std::string real = toMathJaxStr(a.real());
			std::string imag = toMathJaxStr(a.imag());
			if (imag == "0")
				return real;
			else if (real == "0")
				return imag + 'i';
			if (a.imag() > 0)
				return real + '+' + imag + 'i';
			return real + imag + 'i';
		}
		Complex toComplex(std::string str) const 
		{
			if(str.empty() || str.size() > 300)
				return getNan();
			if(size_t ind = std::min(str.find('+', 1), str.find('-', 1)); ind != std::string::npos)
			{
				std::string real = str.substr(0, ind);
				std::string imag = str.substr(ind);
				imag.pop_back();
				if(imag == "-" || imag == "+")
					return Complex(noExceptStold(real), (imag[0] == '+') ? 1 : -1);
				return Complex(noExceptStold(real), noExceptStold(imag));
			}
			else if(str.back() == 'i')
			{
				str.pop_back();
				if(str == "-" || str.empty())
					return Complex(0, str.empty() ? 1 : -1);
				return Complex(0, noExceptStold(str));
			}
			else
				return Complex(noExceptStold(str), 0);
		}

		Complex abs(const Complex& a) const 
		{
			return std::abs(a);
		}
		Complex fmod(const Complex& x, const Complex& y) const 
		{
			Complex quotient = x / y;
			auto a = quotient.real();
			auto b = quotient.imag();
			modf(a, &a);
			modf(b, &b);
			Complex integer_part(a, b);
			Complex res = x - integer_part * y;
			res.imag(std::abs(res.imag()));
			res.real(std::abs(res.real()));
			return res;
		}
		Complex floor(const Complex& a) const 
		{
			return Complex(std::floor(a.real()), std::floor(a.imag()));
		}
		Complex fractional(const Complex& a) const 
		{
			return a - floor(a);
		}
		Complex toRadian(const Complex& a) const 
		{
			return a * Complex(getPi() / Complex(180));
		}
		Complex toDegree(const Complex& a) const 
		{
			return a * Complex(Complex(180) / getPi());
		}
		Complex factorial(const Complex& z, const size_t k = 1000) const 
		{
			auto a = z.real();
			auto b = z.imag();

			if (b == 0.l && a >= 0.l)
				return Complex(tgamma(a + 1));

			decltype(a) s1 = 0.l;
			decltype(a) s2 = 0.l;

			for (std::size_t n = 1; n <= k; ++n)
			{
				s1 += std::log(n) + a * std::log(1.l + 1.l / n) - 0.5l * std::log((n + a) * (n + a) + b * b);
				s2 += b * std::log(1.l + 1.l / n) - 2 * std::atan(b / (n + a + std::sqrt((n + a) * (n + a) + b * b)));
			}

			s1 = std::exp(s1);

			Complex res;

			auto c = s1 * std::cos(s2);
			auto d = s1 * std::sin(s2);

			res.real(c);
			res.imag(d);

			return res;
		}

		Complex exp(const Complex& a) const
		{
			return std::exp(a);	
		}
		Complex ln(const Complex& a) const 
		{
			return std::log(a);
		}
		Complex lg(const Complex& a) const
		{
			return std::log10(a);	
		}
		Complex sqrt(const Complex& a) const 
		{
			return std::sqrt(a);
		}

		Complex pow(const Complex& a, const Complex& exponental) const 
		{
			return std::pow(a, exponental);
		}
		Complex nrt(const Complex& a, const Complex& root) const 
		{
			if(isR(root) && std::fmod(root.real(), 2) == 1 && isR(a) && a.real() < 0)
				return - std::pow(-a.real(), 1 / root.real());
			return std::pow(a, Complex(1) / root);
		}
		Complex log(const Complex& footing, const Complex& b) const 
		{
			return std::log(b) / std::log(footing);
		}

		Complex sin(const Complex& a) const 
		{
			return std::sin(a);
		}
		Complex cos(const Complex& a) const 
		{
			return std::cos(a);
		}
		Complex tg(const Complex& a) const 
		{
			return std::tan(a);
		}
		Complex ctg(const Complex& a) const 
		{
			return Complex(1) / std::tan(a);
		}

		Complex arcsin(const Complex& a) const 
		{
			return std::asin(a);
		}
		Complex arccos(const Complex& a) const 
		{
			return std::acos(a);
		}
		Complex arctg(const Complex& a) const 
		{
			return std::atan(a);
		}
		Complex arcctg(const Complex& a) const 
		{
			return Complex(std::asin(1)) - std::atan(a);
		}

	private:

		using RealType = decltype(Complex().real());

	private:

		bool isR(const Complex& a) const
		{
			return a.imag() == 0.l;	
		}

		RealType noExceptStold(const std::string& str) const
		{
			if(str.empty())
				return getNan().real();
			try
				{
					if(str[0] == '+')
						return stold(str.substr(1));
					return stold(str);
				}
				catch(...)
				{
					return getNan().real();
				}
		}
	
		std::string toStr(const RealType a) const
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

		// Функция для проверки, насколько a близко к рациональному числу p/q
		bool isApproxRational(const RealType a, long long& p, long long& q, const unsigned maxDenominator = 10000, const long double epsilon = 1e-9) const
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

		std::string toMathJaxStr(const RealType a) const
		{
			long long p{}, q{};
			if (!isApproxRational(a, p, q) || q == 1)
				return toStr(a);
			return "\\frac{" + std::to_string(p) + "}{" + std::to_string(q) + "}";
		}

	};

}
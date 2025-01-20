#pragma once

#include<cmath>
#include<string>
#include<limits>
#include<numeric>
#include<ostream>

namespace frac
{

	class Fraction
	{
	public:

		#pragma region constructors

		Fraction() = default;
		template<std::integral type>
		Fraction(const type num, const unsigned long long denom = 1) :
			_num(num), _denom(denom)
		{
			cutBack();
		}
		template<std::floating_point type>
		Fraction(const type val)
		{
			fromLDouble(static_cast<long double>(val));
		}

		#pragma endregion

		#pragma region methods

		const long long numerator() const
		{
			return _num;
		}
		void numerator(const long long num)
		{
			_num = num;
		}

		const unsigned long long denominator() const
		{
			return _denom;
		}
		void denominator(const unsigned long long denom)
		{
			_denom = denom;
		}

		long double toLDouble() const
		{
			return static_cast<long double>(_num) / _denom;
		}

		bool inf() const
		{
			return _num != 0 && _denom == 0;
		}
		bool nan() const
		{
			return _num == 0 && _denom == 0;
		}
		bool negative() const
		{
			return _num < 0;
		}
		bool null() const
		{
			return _num == 0 && _denom != 0;
		}
		bool genaues() const
		{
			return _genaues;
		}

		std::string toString(bool fracMode = false) const
		{
			if (nan())
				return "nan";
			if(inf())
				return _num < 0 ? "-inf" : "inf";
			if(_denom == 1)
				return std::to_string(_num);
			if (!_genaues ||!fracMode)
				return toString(static_cast<long double>(_num) / _denom);
			return std::to_string(_num) + '/' + std::to_string(_denom);
		}
		const std::string toMathJaxString() const
		{
			if (!_genaues)
				return toString(static_cast<long double>(_num) / _denom);
			if (_denom == 1)
				return std::to_string(_num);
			return "\\frac{" + std::to_string(_num) + "}{" + std::to_string(_denom) + '}';
		}

		#pragma endregion

		#pragma region operators

		Fraction& operator+=(const Fraction& other)
		{
			long long num = _num * other._denom + _denom * other._num;
			_denom *= other._denom;
			_num = num;
			cutBack();
			return *this;
		}
		Fraction& operator-=(const Fraction& other)
		{
			long long num = _num * other._denom - _denom * other._num;
			_denom *= other._denom;
			_num = num;
			cutBack();
			return *this;
		}
		Fraction& operator*=(const Fraction& other)
		{
			_num *= other._num;
			_denom *= other._denom;
			cutBack();
			return *this;
		}
		Fraction& operator/=(const Fraction& other)
		{
			_num *= other._denom;
			_denom *= std::abs(other._num);
			if (other._num < 0)
				_num *= -1;
			cutBack();
			return *this;
		}

		Fraction operator+(const Fraction& other) const
		{
			Fraction f = *this;
			f += other;
			return f;
		}
		Fraction operator-(const Fraction& other) const
		{
			Fraction f = *this;
			f -= other;
			return f;
		}
		Fraction operator*(const Fraction& other) const
		{
			Fraction f = *this;
			f *= other;
			return f;
		}
		Fraction operator/(const Fraction& other) const
		{
			Fraction f = *this;
			f /= other;
			return f;
		}

		bool operator==(const Fraction& other) const
		{
			return _num == other._num && _denom == other._denom;
		}
		bool operator!=(const Fraction& other) const
		{
			return !this->operator==(other);
		}
		bool operator<(const Fraction& other) const
		{
			return _num * (long long)other._denom < other._num * (long long)_denom;
		}
		bool operator>(const Fraction& other) const
		{
			return _num * (long long)other._denom > other._num * (long long)_denom;
		}
		bool operator<=(const Fraction& other) const
		{
			return !this->operator>(other);
		}
		bool operator>=(const Fraction& other) const
		{
			return !this->operator<(other);
		}

		Fraction operator-() const
		{
			Fraction pr = *this;
			pr._num *= -1;
			return pr;
		}
		Fraction& operator=(const Fraction& other) = default;

		#pragma endregion

	private:

		long long _num = 0;
		unsigned long long _denom = 1;

		bool _genaues = true;

	private:

		void cutBack()
		{
			if (_num == 0 || _denom == 0)
			{
				_num = _num == 0 ? 0 : 1;
				_denom = _denom == 0 ? 0 : 1;
				_genaues = true;
				return;
			}
			long long gcd = std::gcd(_num, static_cast<long long>(_denom));
			_num /= gcd;
			_denom /= static_cast<unsigned long long>(gcd);
		}

		void fromLDouble(const long double value)
		{
			long double a = value;
			_genaues = true;
			if (a == 0)
			{
				_num = 0;
				_denom = 1;
				return;
			}
			bool neg = a < 0;
			if (neg)
				a *= -1;

			#pragma region standart convert
			//итерационный метод перевода
			unsigned long long q0 = 0, q1 = 1;
			unsigned long long p0 = 1, p1 = 0;
			unsigned long long qart = 0, q = 0, p = 0;

			long double x = 0;
			for (int i = 0; i < 100; ++i)
			{
				x = 1 / a;
				qart = std::floor(x);
				a = x - qart;

				q = qart * q1 + q0;
				p = qart * p1 + p0;

				q0 = q1;
				p0 = p1;
				q1 = q;
				p1 = p;
				if (std::abs(a) < 1e-10)
					break;
			}

			_num = p * (neg ? -1 : 1);
			_denom = q;
			cutBack();
			
			#pragma endregion

			long double pr = (long double)_num / _denom;
			if (std::abs(pr - value) < 1e-10)
				return;
			
			#pragma region special algorithm
			//алгебраический метод конвертации
			_genaues = false;

			unsigned long long n = std::floor(std::log10(LLONG_MAX / value));
				   
			_denom = std::pow(10, n);
			_num = value * _denom;
				
			cutBack();

			#pragma endregion
			pr = (long double)_num / _denom;
			if (std::abs(pr - value) == 0)
				_genaues = true;
		}

		static std::string toString(const long double v)
		{
			std::string res = std::to_string(v);
			if (res.find('.') != std::string::npos)
			{
				while (res.back() == '0')
					res.pop_back();
				if(res.back() == '.')
					res.pop_back();
			}
			return res;
		}

	};

	namespace math
	{
		Fraction abs(const Fraction& a)
		{
			return a.negative() ? -a : a;
		}

		Fraction exp(const Fraction& a)
		{
			return Fraction((long double)std::exp(a.toLDouble()));
		}
		Fraction log(const Fraction& a)
		{
			return Fraction((long double)std::log(a.toLDouble()));
		}
		Fraction log2(const Fraction& a)
		{
			return Fraction((long double)std::log2(a.toLDouble()));
		}
		Fraction log10(const Fraction& a)
		{
			return Fraction((long double)std::log2(a.toLDouble()));
		}

		Fraction sqrt(const Fraction& a)
		{
			if (a.negative())
				return Fraction(0, 0);
			long double sN = std::sqrt((long double)a.numerator());
			long double sD = std::sqrt((long double)a.denominator());

			if ((size_t)sN * (size_t)sN == a.numerator() && (size_t)sD * (size_t)sD == a.denominator())
				return Fraction((size_t)sN, sD);
			return Fraction((long double)(sN) / sD);
		}
		Fraction root(const Fraction& a, unsigned r)
		{
			if (a.negative() && r % 2 == 0)
				return Fraction(0, 0);

			long double sN = std::pow((long double)(std::abs(a.numerator())), 1.l / r);
			long double sD = std::pow((long double)(a.denominator()), 1.l / r);

			if (a.negative())
				sN *= -1;

			long long n = 1, _n = static_cast<long long>(sN);
			unsigned long long m = 1, _d = static_cast<unsigned long long>(sD);
			for (unsigned i = 0; i < r; ++i)
			{
				n *= _n;
				m *= _d;
			}
			if (n == a.numerator() && m == a.denominator())
				return Fraction(_n, _d);
			return Fraction((long double)(sN) / sD);
		}
		Fraction pow(const Fraction& a, const long long d)
		{
			if (d < 0)
				return Fraction(1, 1) / pow(a, -d);

			Fraction res = {1,1};
			Fraction nx = a;

			long long exp = d;

			while (exp != 0)
			{
				if(exp % 2 != 0)
					res *= nx;
				nx *= nx;
				exp /= 2;
			}
			return res;
		}
		Fraction pow(const Fraction& a, const Fraction& d)
		{
			return pow(root(a, d.denominator()), d.numerator());
		}

		Fraction sin(const Fraction& a)
		{
			return Fraction((long double)std::sin(a.toLDouble()));
		}
		Fraction cos(const Fraction& a)
		{
			return Fraction((long double)std::cos(a.toLDouble()));
		}
		Fraction tan(const Fraction& a)
		{
			return Fraction((long double)std::tan(a.toLDouble()));
		}
		Fraction asin(const Fraction& a)
		{
			return Fraction((long double)std::asin(a.toLDouble()));
		}
		Fraction acos(const Fraction& a)
		{
			return Fraction((long double)std::acos(a.toLDouble()));
		}
		Fraction atan(const Fraction& a)
		{
			return Fraction((long double)std::atan(a.toLDouble()));
		}

		Fraction floor(const Fraction& a)
		{
			Fraction res = a;
			res.numerator(res.numerator() - res.numerator() % res.denominator());
			return res;
		}
		Fraction round(const Fraction& a)
		{
			long long pr = a.numerator() % a.denominator();
			pr = std::abs(pr);
			Fraction res = a;
			if(pr >= a.denominator() / 2)
				res.numerator(res.numerator() - res.numerator() % res.denominator() + res.denominator());
			else
				res.numerator(res.numerator() - res.numerator() % res.denominator());
			return res;
		}
	}

	class ComplexFraction
	{
	public:
	
		#pragma region constructor

		ComplexFraction() = default;
		ComplexFraction(Fraction real, Fraction imag) :
			_real{ real }, _imag{ imag } {}
		ComplexFraction(Fraction real) :
			_real{real} {}
	
		#pragma endregion
	
		#pragma region methods

		const Fraction& real() const
		{
			return _real;
		}
		const Fraction& imag() const
		{
			return _imag;
		}

		void real(const Fraction& f)
		{
			_real = f;
		}
		void imag(const Fraction& f)
		{
			_imag = f;
		}

		bool isR() const
		{
			return _imag.null();
		}
		bool null() const
		{
			return _real.null() && _imag.null();
		}

		const std::string toString(bool fracMode = false) const
		{
			if (_imag.null())
				return _real.toString(fracMode);
			if (_real.null())
				return _imag.toString() + 'i';
			std::string res = _real.toString(fracMode);
			res += _imag.negative() ? _imag.toString(fracMode) : '+' + _imag.toString();
			res += 'i';
			return res;
		}

		#pragma endregion

		#pragma region operators

		bool operator==(const ComplexFraction& other) const
		{
			return _real == other._real && _imag == other._imag;
		}
		bool operator!=(const ComplexFraction& other) const
		{
			return !this->operator==(other);
		}

		ComplexFraction& operator=(const ComplexFraction& other) = default;
		ComplexFraction operator-(const ComplexFraction& other)
		{
			ComplexFraction res = *this;
			res._imag *= -1;
			res._real *= -1;
			return res;
		}
	
		ComplexFraction& operator+=(const ComplexFraction& other)
		{
			_real += other._real;
			_imag += other._imag;
			return *this;
		}
		ComplexFraction& operator-=(const ComplexFraction& other)
		{
			_real -= other._real;
			_imag -= other._imag;
			return *this;
		}
		ComplexFraction& operator*=(const ComplexFraction& other)
		{
			Fraction prReal = _real * other._real - _imag * other._imag;
			Fraction prImag = _real * other._imag + _imag * other._real;
			_real = prReal;
			_imag = prImag;
			return *this;
		}
		ComplexFraction& operator/=(const ComplexFraction& other)
		{
			Fraction prDenom = other._real * other._real + other._imag * other._imag;
			Fraction prReal = (_real * other._real + _imag * other._imag) / prDenom;
			Fraction prImag = (_imag * other._real - _real * other._imag) / prDenom;
			_real = prReal;
			_imag = other._imag;
			return *this;
		}

		ComplexFraction operator+(const ComplexFraction& other) const
		{
			ComplexFraction pr = *this;
			pr += other;
			return pr;
		}
		ComplexFraction operator-(const ComplexFraction& other) const
		{
			ComplexFraction pr = *this;
			pr -= other;
			return pr;
		}
		ComplexFraction operator*(const ComplexFraction& other) const
		{
			ComplexFraction pr = *this;
			pr *= other;
			return pr;
		}
		ComplexFraction operator/(const ComplexFraction& other) const
		{
			ComplexFraction pr = *this;
			pr /= other;
			return pr;
		}

		#pragma endregion

	private:

		Fraction _real{};
		Fraction _imag{};

	};

	namespace math
	{
		Fraction abs(const ComplexFraction& x)
		{
			return sqrt(x.real() * x.real() + x.imag() * x.imag());
		}
		Fraction arg(const ComplexFraction& x)
		{
			//asin(1) = pi/2
			//acos(-1) = pi
			if (x.real().null())
			{
				if (x.imag().null())
					return 0;
				if (x.imag().negative())
					return -asin(1);
				return asin(1);
			}
			if (x.real().negative())
			{
				if (x.imag().negative())
					return atan(x.imag() / x.real()) - acos(-1);
				return atan(x.imag() / x.real()) + acos(-1);
			}
			return atan(x.imag() / x.real());
		}

		ComplexFraction exp(const ComplexFraction& x)
		{
			Fraction real = exp(x.real());
			return ComplexFraction(real * cos(x.imag()), real * sin(x.imag()));
		}
		ComplexFraction log(const ComplexFraction& x)
		{
			return ComplexFraction(log(abs(x)), arg(x));
		}
		ComplexFraction log2(const ComplexFraction& x)
		{
			return log(x) / log(2);
		}
		ComplexFraction log10(const ComplexFraction& x)
		{
			return log(x) / log(10);
		}

		ComplexFraction sqrt(const ComplexFraction& x)
		{
			const Fraction z = abs(x);
			return ComplexFraction(sqrt((z + x.real()) / 2), sqrt((z - x.real()) / 2) * (x.imag().negative() ? -1 : 1));
		}
		ComplexFraction root(const ComplexFraction& a, unsigned r)
		{
			if (a.isR())
				return root(a.real(), r);
			Fraction phi = arg(a) / r;
			Fraction pr = root(abs(a),r);
			return ComplexFraction(pr * cos(phi), pr * sin(phi));
		}
		ComplexFraction pow(const ComplexFraction& a, const long long d)
		{
			if (d < 0)
				return ComplexFraction(1, 0) / pow(a, -d);

			ComplexFraction res = { 1,1 };
			ComplexFraction nx = a;

			long long exp = d;

			while (exp != 0)
			{
				if (exp % 2 != 0)
					res *= nx;
				nx *= nx;
				exp /= 2;
			}
			return res;
		}
		ComplexFraction pow(const ComplexFraction& a, const Fraction& d)
		{
			return pow(root(a, d.denominator()), d.numerator());
		}
		ComplexFraction pow(const ComplexFraction& a, const ComplexFraction& d)
		{
			Fraction r = abs(a);
			Fraction f = arg(a);

			Fraction phi = d.imag() * log(r) + d.real() * f;
			Fraction pr = pow(r, d.real()) / exp(f * d.imag());
			return ComplexFraction(pr * cos(phi), pr * sin(phi));
		}

		ComplexFraction sin(const ComplexFraction& a)
		{
			ComplexFraction e = exp(ComplexFraction(0, 1) * a);
			return (e - ComplexFraction(1, 0) / e) / ComplexFraction(0, 2);
		}
		ComplexFraction cos(const ComplexFraction& a)
		{
			ComplexFraction e = exp(ComplexFraction(0, 1) * a);
			return (e + ComplexFraction(1, 0) / e) / ComplexFraction(2, 0);
		}
		ComplexFraction tan(const ComplexFraction& a)
		{
			ComplexFraction e = exp(ComplexFraction(0, 1) * a);
			ComplexFraction me = ComplexFraction(1, 0) / e;
			return (e - me) / ((e + me) * ComplexFraction(0, 1));
		}

		ComplexFraction asin(const ComplexFraction& a)
		{
			return ComplexFraction(0, -1) * log(ComplexFraction(0, 1) * a + sqrt(ComplexFraction(1,0) - a * a));
		}
		ComplexFraction acos(const ComplexFraction& a)
		{
			return ComplexFraction(0, -1) * log(a + ComplexFraction(0, 1) * sqrt(ComplexFraction(1, 0) - a * a));
		}
		ComplexFraction atan(const ComplexFraction& a)
		{
			Fraction pr = Fraction(1) - a.imag();
			return ComplexFraction(0, { 1,2 }) * log(ComplexFraction(1, -Fraction(2) * a.real() * pr / (pr + a.real() * a.real())));
		}
	}

	std::ostream& operator<<(std::ostream& out, const Fraction& a)
	{
		out << a.toString();
		return out;
	}
	std::ostream& operator<<(std::ostream& out, const ComplexFraction& a)
	{
		out << a.toString();
		return out;
	}

}

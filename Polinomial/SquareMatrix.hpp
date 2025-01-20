#pragma once

#include<iostream>
#include<vector>
#include<initializer_list>

namespace expr
{

	template<typename Type>
	class SquareMatrix
	{
	public:

		#pragma region constructors

		SquareMatrix() = default;
		SquareMatrix(size_t width, const Type& value = Type(0)) :
			_matrix(std::move(std::vector<std::vector<Type>>(width, std::move(std::vector<Type>(width, value)))))
		{}
		SquareMatrix(const std::initializer_list<std::vector<Type>>& list) :
			_matrix(list)
		{}
		SquareMatrix(const SquareMatrix& other) : 
			_matrix(other._matrix)
		{}
		SquareMatrix(SquareMatrix&& other) :
			_matrix(std::move(other._matrix))
		{}
		~SquareMatrix() = default;
	
		#pragma endregion

		#pragma region methods

		size_t getSize() const
		{
			return _matrix.size();
		}

		void clear() const
		{
			_matrix.clear();
		}

		SquareMatrix<Type> minorMatrix(size_t y, size_t x) const
		{
			if (_matrix.size() < 2)
				return SquareMatrix<Type>{};
			SquareMatrix<Type> minor(_matrix.size() - 1);

			bool xP = false, yP = false;

			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				if (i == y)
				{
					yP = true;
					continue;
				}
				xP = false;
				for (size_t j = 0; j < _matrix.size(); ++j)
				{
					if (j == x)
					{
						xP = true;
						continue;
					}
					minor._matrix[i - yP][j - xP] = _matrix[i][j];
				}
			}
			return minor;
		}

		Type minor(size_t y, size_t x) const
		{
			return minorMatrix(y, x).determinant();
			/*if (_matrix.size() < 2)
				return Type(0);
			SquareMatrix<Type> minor(_matrix.size() - 1);

			bool xP = false, yP = false;

			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				if (i == y)
				{
					yP = true;
					continue;
				}
				for (size_t j = 0; j < _matrix.size(); ++j)
				{
					if (j == x)
					{
						xP = true;
						continue;
					}
					minor._matrix[i - yP][j - xP] = _matrix[i][j];
				}
			}
			return minor.determinant();*/
		}

		Type determinant() const
		{
			if (_matrix.size() <= 1)
			{
				if (_matrix.size() == 0)
					return Type(0);
				if (_matrix.size() == 1)
					return _matrix[0][0];
				if (_matrix.size() == 2)
					return _matrix[0][0] * _matrix[1][1] - _matrix[0][1] * _matrix[1][0];
				if (_matrix.size() == 3)
					return
					_matrix[0][0] * _matrix[1][1] * _matrix[2][2] +
					_matrix[0][1] * _matrix[1][2] * _matrix[2][0] +
					_matrix[0][2] * _matrix[1][0] * _matrix[2][1] -
					(
						_matrix[0][2] * _matrix[1][1] * _matrix[2][0] +
						_matrix[0][1] * _matrix[1][0] * _matrix[2][2] +
						_matrix[0][0] * _matrix[1][2] * _matrix[2][1]
					);
			}
			Type res = Type(0);
			const Type null = Type(0);
			Type mul = Type(1);
			Type pr = Type(-1);
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				if(_matrix[i][0] != null)
				res += mul * _matrix[i][0] * minor(i, 0);
				mul *= pr;
			}
			return res;
		}

		#pragma endregion

		#pragma region operators

		SquareMatrix& operator=(const SquareMatrix& other) = default;
		SquareMatrix& operator=(SquareMatrix&& other)
		{
			_matrix = std::move(other._matrix);
			return *this;
		}

		const Type& operator()(size_t y, size_t x) const
		{
			return _matrix[y][x];
		}
		Type& operator()(size_t y, size_t x)
		{
			return _matrix[y][x];
		}

		bool operator==(const SquareMatrix& other) const
		{
			if (other._matrix.size() != _matrix.size())
				return false;
			for (size_t i = 0; i < _matrix.size(); ++i)
				for (size_t j = 0; j < _matrix.size(); ++j)
					if (_matrix[i][j] != other._matrix[i][j])
						return false;
			return true;
		}
		bool operator!=(const SquareMatrix& other) const
		{
			return !this->operator==(other);
		}

		SquareMatrix operator-() const
		{
			SquareMatrix res = *this;
			for (size_t i = 0; i < _matrix.size(); ++i)
				for (size_t j = 0; j < _matrix.size(); ++j)
					res._matrix[i][j] *= Type(-1);
			return res;
		}

		SquareMatrix& operator+=(const SquareMatrix& other)
		{
			if (other._matrix.size() != _matrix.size())
				return *this;
			for (size_t i = 0; i < _matrix.size(); ++i)
				for (size_t j = 0; j < _matrix.size(); ++j)
					_matrix[i][j] += other._matrix[i][j];
			return *this;
		}
		SquareMatrix& operator-=(const SquareMatrix& other)
		{
			if (other._matrix.size() != _matrix.size())
				return *this;
			for (size_t i = 0; i < _matrix.size(); ++i)
				for (size_t j = 0; j < _matrix.size(); ++j)
					_matrix[i][j] -= other._matrix[i][j];
			return *this;
		}
		SquareMatrix& operator*=(const SquareMatrix& other)
		{
			if (other._matrix.size() != _matrix.size())
				return *this;
			this->operator=(this->operator*(other));
			return *this;
		}

		SquareMatrix operator*(const SquareMatrix& other) const
		{
			if (other._matrix.size() != _matrix.size())
				return SquareMatrix();
			SquareMatrix res(_matrix.size());

			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				for (size_t j = 0; j < _matrix.size(); ++j)
				{
					Type item = Type(0);
					for (size_t k = 0; k < _matrix.size(); ++k)
					{
						item += _matrix[i][k] * other._matrix[k][j];
					}
					res._matrix[i][j] = item;
				}
			}
			return res;
		}
		SquareMatrix operator+(const SquareMatrix& other) const
		{
			if (other._matrix.size() != _matrix.size())
				return SquareMatrix();
			SquareMatrix res = *this;
			res += other;
			return res;
		}
		SquareMatrix operator-(const SquareMatrix& other) const
		{
			if (other._matrix.size() != _matrix.size())
				return SquareMatrix();
			SquareMatrix res = *this;
			res -= other;
			return res;
		}

		friend std::ostream& operator<<(std::ostream& out, const SquareMatrix& other)
		{
			for (size_t i = 0; i < other._matrix.size(); ++i)
			{
				out << '|' << other._matrix[i][0];
				for (size_t j = 1; j < other._matrix.size(); ++j)
					out << '\t' << other._matrix[i][j];
				out << '|' << '\n';
			}
			return out;
		}

		#pragma endregion

	private:

		std::vector<std::vector<Type>> _matrix;

	};

}
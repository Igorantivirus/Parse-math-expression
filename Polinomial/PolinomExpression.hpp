#pragma once

#include"Polinomial.hpp"

namespace expr
{

	template<typename Complex>
	class PolinomExpression
	{
	public:
		PolinomExpression() = default;

		void clear()
		{
			_pols.clear();
		}

		void add(const Polinomial<Complex>& pol, const ActionT act)
		{
			_pols.push_back({pol, act});
		}

		const std::pair<Polinomial<Complex>, ActionT>& back() const
		{
			return _pols.back();
		}

		Polinomial procession() const
		{
			return Polinomial<Complex>;
		}

	private:

		std::vector<std::pair<Polinomial<Complex>, ActionT>> _pols;

	};


}
#pragma once

#include"BaseClasses.hpp"
#include"Value.hpp"

namespace expr
{
	template<typename Complex>
	class Expression : public MathBase
	{
	public:
		Expression() : MathBase() {}
		Expression(const ActionT act) : MathBase(act) {}
		Expression(const Expression& other) : MathBase(other), _type(other._type)
		{
			for (const auto& i : other._expression)
				_expression.push_back(i->copy());
		}

		Expression operator=(const Expression& other)
		{
			_act = other._act;
			_type = other._type;

			_expression.clear();
			for (const auto& i : other._expression)
				_expression.push_back(i->copy());

			return *this;
		}

		const std::string toStr() const override
		{
			std::string res;
			
			if(_expression.size() > 1 || _type != BracketT::standart)
				res = mconverter.toStrOpen(_type);

			for (auto& i : _expression)
				if (i != nullptr)
					res += i->toStr() + mconverter.toStr(i->getAct());
			if (_expression.size() > 1 || _type != BracketT::standart)
				res += mconverter.toStrClose(_type);
			return res;
		}
		const std::string toMathJaxStr() const override
		{
			std::string res;

			if (_expression.size() > 1 || _type != BracketT::standart)
				res += (_type == BracketT::whole || _type == BracketT::fract ? "\\" : "") + mconverter.toStrOpen(_type);

			for (size_t i = 0; i < _expression.size(); ++i)
			{
				if (_expression[i]->getAct() == ActionT::div)
				{
					res += "\\frac{" + _expression[i]->toMathJaxStr() + "}{";
					if (i + 1 < _expression.size())
						res += _expression[i + 1]->toMathJaxStr() + '}' + mconverter.toStr(_expression[i + 1]->getAct());
					else
						res += '}';
					++i;
				}
				else if (i - 1 < _expression.size() && _expression[i - 1]->getAct() == ActionT::pow)
				{
					res += '{' + _expression[i]->toMathJaxStr() + '}' + mconverter.toStr(_expression[i]->getAct());
				}
				else
				{
					res += _expression[i]->toMathJaxStr() + mconverter.toStr(_expression[i]->getAct());
				}
			}

			if (_expression.size() > 1 || _type != BracketT::standart)
				res += (_type == BracketT::whole || _type == BracketT::fract ? "\\" : "") + mconverter.toStrClose(_type);

			return res;
		}

		MathBasePtr getProcessedValue() const override
		{
			std::vector<Value<Complex>> values;
			for (const auto& i : _expression)
			{
				if (i != nullptr)
					values.push_back(Value<Complex>::convertProcessed(*i));
			}

			processAt(values, { ActionT::hiddMultiply });
			processAt(values, { ActionT::pow });
			processAt(values, { ActionT::multiply, ActionT::div, ActionT::wholeDiv, ActionT::mod });
			processAt(values, { ActionT::plus, ActionT::minus });

			return std::make_unique<Value<Complex>>(Value<Complex>(solver::solve<Complex>(values[0].getValue(), _type), _act));
		}
		MathBasePtr getProcessedEpression() const override
		{
			std::vector<MathBasePtr> newExpr;
			for (const auto& i : _expression)
				if (i != nullptr)
					values.push_back(std::move(i->getProcessedEpression()));






		}
		MathBasePtr copy() const override
		{
			return std::make_unique<Expression>(*this);
		}

		BracketT getBracket() const
		{
			return _type;
		}
		void setBracket(const BracketT type)
		{
			_type = type;
		}

		void add(const MathBasePtr& arg)
		{
			_expression.push_back(arg->copy());
		}
		void add(const MathBase& arg)
		{
			_expression.push_back(arg.copy());
		}

		void addWith(const MathBasePtr& arg, const ActionT act)
		{
			_expression.push_back(arg->copy());
			_expression.back()->setAct(act);
		}
		void addWith(const MathBase& arg, const ActionT act)
		{
			_expression.push_back(arg.copy());
			_expression.back()->setAct(act);
		}

		MathBasePtr operator[](const size_t ind) const
		{
			return _expression[ind]->copy();
		}
		MathBasePtr& operator[](const size_t ind)
		{
			return _expression[ind];
		}

		const size_t getSize() const
		{
			return _expression.size();
		}

	private:

		std::vector<MathBasePtr> _expression;

		BracketT _type = BracketT::standart;

	private:

		void processAt(std::vector<Value<Complex>>& vls, const std::vector<ActionT>& acts) const
		{
			auto have = [&acts](const ActionT& act)
			{
				for (const auto& i : acts)
					if (i == act)
						return true;
				return false;
			};
			Complex prv;
			ActionT act;
			for (size_t i = 0; i < vls.size() - 1; ++i)
			{
				if (have(vls[i].getAct()))
				{
					prv = solver::solve(vls[i].getValue(), vls[i + 1].getValue(), vls[i].getAct());
					act = vls[i + 1].getAct();
					vls.erase(vls.begin() + i, vls.begin() + i + 2);
					vls.insert(vls.begin() + i, Value(prv, act));
					--i;
				}
			}
		}

		void processExprPow(std::vector<MathBasePtr>& newExpr)
		{
			for (size_t i = 0; i < newExpr.size(); ++i)
			{

			}
		}

		const MathType getMT() const override
		{
			return MathBase::MathType::expression;
		}

	};
}

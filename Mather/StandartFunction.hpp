#pragma once

#include"BaseClasses.hpp"
#include"Value.hpp"

namespace expr
{

	template<typename Complex>
	class StanartFunction : public Function
	{
	public:
		StanartFunction() : Function() {}
		StanartFunction(const ActionT action) : Function(action) {}
		StanartFunction(const MathBase& argument, const FunctionT type, const ActionT action = ActionT::none) : Function(argument, action), _type(type) {}
		StanartFunction(const MathBasePtr& argument, const FunctionT type, const ActionT action = ActionT::none) : Function(argument->copy(), action), _type(type) {}
		StanartFunction(const StanartFunction& other) : Function(other), _type(other._type) {}

		const std::string toStr() const override
		{
			std::string arg = _argument == nullptr ? "nan" : _argument->toStr();
			return mconverter.toStr(_type) + '(' + arg + ')';
		}
		const std::string toMathJaxStr() const override
		{
			std::string arg = _argument == nullptr ? "nan" : _argument->toMathJaxStr();
			if (_type == FunctionT::sqrt)
				return "\\" + mconverter.toStr(_type) + '{' + arg + '}';
			return "\\" + mconverter.toStr(_type) + '(' + arg + ')';
		}

		MathBasePtr getProcessedValue() const override
		{
			Value v = Value<Complex>::convertProcessed(*_argument);
			return std::make_unique<Value<Complex>>(Value(solver::solve(v.getValue(), _type), _act));
		}
		MathBasePtr getProcessedEpression() const override
		{
			MathBasePtr myArg = _argument->getProcessedEpression();
			if (myArg->getMT() == MathBase::MathType::function)
			{
				StanartFunction arg = *dynamic_cast<StanartFunction*>(myArg.get());
				if (getOpposite(arg._type) != FunctionT::none)
					return arg._argument->copy();
			}
			StanartFunction res(_act);
			res._argument = std::move(myArg);
			return res->copy();
		}
		MathBasePtr copy() const override
		{
			return std::make_unique<StanartFunction>(*this);
		}

		const FunctionT getType() const
		{
			return _type;
		}
		void setType(const FunctionT type)
		{
			_type = type;
		}

	private:

		FunctionT _type = FunctionT::none;

	private:

		const MathType getMT() const override
		{
			return MathBase::MathType::function;
		}

	};

}
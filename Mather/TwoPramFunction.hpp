#pragma once

#include"BaseClasses.hpp"
#include"MathConverter.hpp"

namespace expr
{

	template<typename Complex>
	class TwoPramFunction : public Function
	{
	public:
		TwoPramFunction() : Function() {}
		TwoPramFunction(const ActionT action) : Function(action) {}
		TwoPramFunction(const MathBase& arg1, const MathBase& arg2, const TwoFunctionT type, const ActionT act = ActionT::none) : Function(arg1, act), _type(type), _specialArg(arg2.copy()) {}
		TwoPramFunction(const MathBasePtr& arg1, const MathBasePtr& arg2, const TwoFunctionT type, const ActionT act = ActionT::none) : Function(arg1->copy(), act), _type(type), _specialArg(arg2->copy()) {}
		TwoPramFunction(const TwoPramFunction& other) : Function(other), _type(other._type), _specialArg(other._specialArg->copy()) {}

		const std::string toStr() const override
		{
			std::string arg = _argument == nullptr ? "nan" : _argument->toStr();
			std::string sarg = _specialArg == nullptr ? "nan" : _specialArg->toStr();
			if (_type == TwoFunctionT::pow)
				return arg + '^' + sarg;
			return mconverter.toStr(_type) + '(' + sarg + "," + arg + ')';
		}
		const std::string toMathJaxStr() const override {
			std::string arg = _argument == nullptr ? "nan" : _argument->toMathJaxStr();
			std::string sarg = _specialArg == nullptr ? "nan" : _specialArg->toMathJaxStr();
			if (_type == TwoFunctionT::log)
				return "\\log_{" + sarg + "}{(" + arg + ")}";
			if (_type == TwoFunctionT::root)
				return "\\sqrt[" + sarg + "]{" + arg + "}";
			if (_type == TwoFunctionT::root)
				return "\\{" + arg + "}^{" + sarg + "}";
			return "nan";
		}

		MathBasePtr getProcessedValue() const override
		{
			Value<Complex> arg = Value<Complex>::convertProcessed(*_argument);
			Value<Complex> sarg = Value<Complex>::convertProcessed(*_specialArg);
			return std::make_unique<Value<Complex>>(Value(solver::solve(arg.getValue(), sarg.getValue(), _type), _act));
		}
		MathBasePtr copy() const override
		{
			return std::make_unique<TwoPramFunction>(*this);
		}

		TwoFunctionT getType() const
		{
			return _type;
		}
		void setType(const TwoFunctionT type)
		{
			_type = type;
		}

		void setSpecilAgrument(const MathBase& arg)
		{
			_specialArg = arg.copy();
		}
		void setSpecilAgrument(const MathBasePtr& arg)
		{
			_specialArg = arg->copy();
		}

	private:

		TwoFunctionT _type = TwoFunctionT::none;

		std::unique_ptr<MathBase> _specialArg;

	private:

		const MathType getMT() const override
		{
			return MathBase::MathType::twoFunction;
		}

	};

}
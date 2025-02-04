#pragma once

#include"BaseClasses.hpp"
#include"MathConverter.hpp"

namespace expr
{

	template<typename Complex>
	class PostfixFunction : public Function
	{
	public:
		PostfixFunction() : Function() {}
		PostfixFunction(const ActionT action) : Function(action) {}
		PostfixFunction(const MathBase& argument, const PostfixFunctionT type, const ActionT action = ActionT::none) : Function(argument, action), _type(type) {}
		PostfixFunction(const MathBasePtr& argument, const PostfixFunctionT type, const ActionT action = ActionT::none) : Function(argument->copy(), action), _type(type) {}
		PostfixFunction(const PostfixFunction& other) : Function(other), _type(other._type) {}

		const std::string toStr() const override
		{
			std::string arg = _argument == nullptr ? "nan" : _argument->toStr();
			return arg + MathConverter::singl().toStr(_type);
		}
		const std::string toMathJaxStr() const override
		{
			std::string arg = _argument == nullptr ? "nan" : _argument->toMathJaxStr();
			return arg + MathConverter::singl().toStr(_type);
		}

		const PostfixFunctionT getType() const
		{
			return _type;
		}
		void setType(const PostfixFunctionT type)
		{
			_type = type;
		}

		MathBasePtr getProcessedValue() const override
		{
			Value v = Value<Complex>::convertProcessed(*_argument);
			return std::make_unique<Value<Complex>>(Value(solver::solve(v.getValue(), _type), _act));
		}
		MathBasePtr copy() const override
		{
			return std::make_unique<PostfixFunction>(*this);
		}

	private:

		PostfixFunctionT _type = PostfixFunctionT::none;

	private:

		const MathType getMT() const override
		{
			return MathBase::MathType::postfixFunction;
		}

	};

}
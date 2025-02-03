#pragma once

#include"MathUtils.hpp"

namespace expr
{

	class MathBase
	{
	protected:

		enum class MathType : char
		{
			none = 0,
			value,
			function,
			postfixFunction,
			twoFunction,
			expression,
			variable
		};

		virtual const MathType getMT() const = 0;

	public:
		MathBase() = default;
		MathBase(const ActionT ActionT) : _act(ActionT) {}
		MathBase(const MathBase& other) : _act(other._act) {}
		virtual ~MathBase() = default;

		virtual const std::string toStr() const = 0;
		virtual const std::string toMathJaxStr() const = 0;

		virtual std::unique_ptr<MathBase> getProcessedValue() const = 0;
		virtual std::unique_ptr<MathBase> copy() const = 0;

		ActionT getAct() const
		{
			return _act;
		}
		void setAct(const ActionT act)
		{
			_act = act;
		}

	protected:

		ActionT _act = ActionT::none;

	};

	using MathBasePtr = std::unique_ptr<MathBase>;

	class Function : public MathBase
	{
	public:
		Function() : MathBase() {}
		Function(const ActionT act) : MathBase(act) {}
		Function(const MathBase& argument, const ActionT act = ActionT::none) : MathBase(act), _argument(argument.copy()) {}
		Function(const MathBasePtr& argument, const ActionT act = ActionT::none) : MathBase(act), _argument(argument->copy()) {}
		Function(const Function& other) : MathBase(other._act), _argument(other._argument->copy()) {}

		MathBasePtr getArgument() const
		{
			return _argument->copy();
		}
		void setArgument(const MathBasePtr& argument)
		{
			_argument = argument->copy();
		}
		void setArgument(const MathBase& argument)
		{
			_argument = argument.copy();
		}

	protected:

		MathBasePtr _argument;

	};

}
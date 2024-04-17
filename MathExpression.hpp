#pragma once

#include<string>
#include<vector>
#include<memory>
#include<math.h>

#include"ParseUtils.hpp"

namespace expr
{
	class ExpressionBase
	{
	public:
		friend class Value;
		friend class Function;
		friend class Expression;
	public:
		ExpressionBase() = default;
		ExpressionBase(const Action& action) : next(action) {};

		virtual std::unique_ptr<ExpressionBase> processed() const = 0;

		const Action getNextAction() const
		{
			return next;
		}
		void setNextAction(const Action a)
		{
			next = a;
		}

		virtual void print() const = 0;

	protected:

		Action next = Action::none;

		virtual std::unique_ptr<ExpressionBase> copy() const = 0;
	};


	class Value : public ExpressionBase
	{
	public:
		Value() : ExpressionBase() {}
		Value(const FType& nv) : ExpressionBase(), value(nv) {}
		Value(const FType& nv, const Action na) : ExpressionBase(na), value(nv) {}
		Value(const Value& other) : ExpressionBase(other.next), value(other.value) {}

		Value& operator=(const Value& other)
		{
			if (this == &other)
				return *this;

			value = other.value;
			next = other.next;

			return *this;
		}
		Value& operator=(const FType& v)
		{
			value = v;
			return *this;
		}

		std::unique_ptr<ExpressionBase> processed() const override
		{
			return std::make_unique<Value>(*this);
		}

		const FType& getValue() const
		{
			return value;
		}
		void setValue(const FType& nv)
		{
			value = nv;
		}

		void print() const override
		{
			std::cout << value;
			enumFunc::outAct(next);
		}

	private:
		FType value = 0;

		std::unique_ptr<ExpressionBase> copy() const override
		{
			return std::make_unique<Value>(*this);
		}
	};

	class Function : public ExpressionBase
	{
	public:
		Function() : ExpressionBase() {}
		Function(const FunctionType t) : ExpressionBase(), type(t) {}
		Function(const Function& other) : ExpressionBase(other.next), type(other.type)
		{
			if (other.arg != nullptr)
				arg = other.arg->copy();
		}

		Function& operator=(const Function& other)
		{
			if (this == &other)
				return *this;
			type = other.type;
			next = other.next;
			arg.reset();
			if (other.arg != nullptr)
				arg = other.arg->copy();
			return *this;
		}

		std::unique_ptr<ExpressionBase> processed() const override
		{
			if (arg == nullptr)
				return std::make_unique<Value>(Value(0, next));
			std::unique_ptr<ExpressionBase> pr = arg->processed();
			Value* valuePtr = nullptr;
			if ((valuePtr = dynamic_cast<Value*>(pr.get())) == nullptr)
				return std::make_unique<Value>(Value(0, next));
			return std::make_unique<Value>(Value(processingFunction(valuePtr->getValue()), next));
		}

		const FunctionType getType() const
		{
			return type;
		}
		void setType(const FunctionType t)
		{
			type = t;
		}

		const std::unique_ptr<ExpressionBase>& getArg() const
		{
			return arg;
		}

		void setArg(const ExpressionBase& value)
		{
			arg = value.copy();
		}

		void print() const override
		{
			enumFunc::outFunc(type);
			std::cout << '(';
			if (arg != nullptr)
				arg->print();
			std::cout << ')';
			enumFunc::outAct(next);
		}

	private:

		FunctionType type = FunctionType::none;

		std::unique_ptr<ExpressionBase> arg;

		std::unique_ptr<ExpressionBase> copy() const override
		{
			return std::make_unique<Function>(*this);
		}

		const FType processingFunction(const FType& v) const
		{
			switch (type)
			{
			case FunctionType::sqrt:
				return std::sqrt(v);
			case FunctionType::ln:
				return std::log(v);
			case FunctionType::lg:
				return std::log10(v);
			case FunctionType::sin:
				return std::sin(v);
			case FunctionType::cos:
				return std::cos(v);
			case FunctionType::tg:
				return std::tan(v);
			case FunctionType::ctg:
				return FType(1) / std::tan(v);
			case FunctionType::arcsin:
				return std::asin(v);
			case FunctionType::arccos:
				return std::acos(v);
			case FunctionType::arctg:
				return std::atan(v);
			case FunctionType::arcctg:
				return FType(PI_val / 2) - std::atan(v);
			case FunctionType::fact:
				return myMath::factorial(v);
			case FunctionType::degrees:
				return myMath::toRadian(v);
			case FunctionType::radian:
				return myMath::toDegrees(v);
			default:
				return 0;
			}
		}

	};

	class Expression : public ExpressionBase
	{
	public:
		Expression() : ExpressionBase() {}
		Expression(const Expression& other) : ExpressionBase(other.next), brk(other.brk)
		{
			for (const auto& i : other.values)
				if (i != nullptr)
					values.push_back(i->copy());
		}

		Expression& operator=(const Expression& other)
		{
			if (this == &other)
				return *this;

			next = other.next;
			values.clear();
			for (const auto& i : other.values)
				if (i != nullptr)
					values.push_back(i->copy());

			return *this;
		}

		std::unique_ptr<ExpressionBase> processed() const override
		{
			if (values.empty())
				return std::make_unique<Value>(Value(0, Action::plus));

			std::vector<Value> vls;

			for (const auto& i : values)
				if (i != nullptr)
					vls.push_back(*dynamic_cast<Value*>(i->processed().get()));

			vls.back().setNextAction(Action::none);

			firstProcessed(vls);
			secondProcessed(vls);
			thirdProcessed(vls);

			if (brk == Brackets::round)
				vls[0].setValue(myMath::floor(vls[0].getValue()));
			if (brk == Brackets::frac)
				vls[0].setValue(myMath::frac(vls[0].getValue()));
			if (brk == Brackets::modul)
				vls[0].setValue(std::abs(vls[0].getValue()));
			vls[0].setNextAction(next);
			return std::make_unique<Value>(vls[0]);
		}

		Value getProcessingResult() const
		{
			std::unique_ptr<ExpressionBase> expr = processed();
			return *dynamic_cast<Value*>(expr.get());
		}

		void add(const ExpressionBase& expr)
		{
			values.push_back(std::move(expr.copy()));
		}

		const Brackets getBrackets() const
		{
			return brk;
		}
		void setBrackets(const Brackets nbr)
		{
			brk = nbr;
		}

		void print() const override
		{
			enumFunc::outOpenBrack(brk);
			for (const auto& i : values)
				i->print();
			enumFunc::outCloseBrack(brk);
			enumFunc::outAct(next);
		}

	private:
		Brackets brk = Brackets::none;

		std::vector<std::unique_ptr<ExpressionBase>> values;

		std::unique_ptr<ExpressionBase> copy() const override
		{
			return std::make_unique<Expression>(*this);
		}

		void firstProcessed(std::vector<Value>& vls) const
		{
			Value pr;
			for (size_t i = 0; i < vls.size() - 1; ++i)
			{
				if (enumFunc::firstAction(vls[i].getNextAction()))
				{
					if (vls[i].getNextAction() == Action::pow)
						pr.setValue(std::pow(vls[i].getValue(), vls[i + 1].getValue()));
					else if (vls[i].getNextAction() == Action::nrt)
						pr.setValue(std::pow(vls[i + 1].getValue(), FType(1) / vls[i].getValue()));
					else if (vls[i].getNextAction() == Action::log)
						pr.setValue(std::log(vls[i + 1].getValue()) / std::log(vls[i].getValue()));
					pr.setNextAction(vls[i + 1].getNextAction());
					vls[i] = pr;
					vls.erase(vls.begin() + i + 1);
					i--;
				}
			}
		}
		void secondProcessed(std::vector<Value>& vls) const
		{
			Value pr;
			for (size_t i = 0; i < vls.size() - 1; ++i)
			{
				if (enumFunc::secondAction(vls[i].getNextAction()))
				{
					if (vls[i].getNextAction() == Action::mult)
						pr.setValue(vls[i].getValue() * vls[i + 1].getValue());
					else if (vls[i].getNextAction() == Action::div)
						pr.setValue(vls[i].getValue() / vls[i + 1].getValue());
					else if (vls[i].getNextAction() == Action::rdiv)
						pr.setValue(myMath::fmod(vls[i].getValue(), vls[i + 1].getValue()));
					else if (vls[i].getNextAction() == Action::idiv)
						pr.setValue(myMath::floor(vls[i].getValue() / vls[i + 1].getValue()));

					pr.setNextAction(vls[i + 1].getNextAction());
					vls[i] = pr;
					vls.erase(vls.begin() + i + 1);
					i--;
				}
			}
		}
		void thirdProcessed(std::vector<Value>& vls) const
		{
			Value pr;
			for (size_t i = 0; i < vls.size() - 1; ++i)
			{
				if (enumFunc::thirdAction(vls[i].getNextAction()))
				{
					if (vls[i].getNextAction() == Action::plus)
						pr.setValue(vls[i].getValue() + vls[i + 1].getValue());
					else if (vls[i].getNextAction() == Action::minus)
						pr.setValue(vls[i].getValue() - vls[i + 1].getValue());

					pr.setNextAction(vls[i + 1].getNextAction());
					vls[i] = pr;
					vls.erase(vls.begin() + i + 1);
					i--;
				}
			}
		}

	};

}

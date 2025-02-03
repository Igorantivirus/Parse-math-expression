#pragma once

#include"BaseClasses.hpp"
#include"MathConverter.hpp"

namespace expr
{

	template<typename Complex>
	class Value : public MathBase
	{
	public:
		Value() : MathBase() {}
		Value(const Value& other) : MathBase(other), _value(other._value) {};
		Value(const ActionT action) : MathBase(action) {}
		Value(const Complex& value, const ActionT act = ActionT::none) : MathBase(act), _value(value) {}

		const std::string toStr() const override
		{
			mathWorker::MathWorker<Complex> worker;
			return worker.toStr(_value);
		}
		const std::string toMathJaxStr() const override
		{
			mathWorker::MathWorker<Complex> worker;
			return worker.toStr(_value);
		}

		MathBasePtr getProcessedValue() const override
		{
			return std::make_unique<Value>(*this);
		}
		MathBasePtr copy() const override
		{
			return std::make_unique<Value>(*this);
		}

		const Complex getValue() const
		{
			return _value;
		}
		void setComplexValue(const Complex& value)
		{
			_value = value;
		}

		static Value convertProcessed(const MathBase& arg)
		{
			mathWorker::MathWorker<Complex> worker;
			std::unique_ptr<MathBase> pr = arg.getProcessedValue();
			Value* valuePtr = nullptr;
			if ((valuePtr = dynamic_cast<Value*>(pr.get())) == nullptr)
				return Value(worker.getNan(), arg.getAct());
			return *valuePtr;
		}
		static Value convertProcessed(const MathBasePtr& arg)
		{
			if (arg == nullptr)
				return Value(Complex(0));
			mathWorker::MathWorker<Complex> worker;
			std::unique_ptr<MathBase> pr = arg->getProcessedValue();
			Value* valuePtr = nullptr;
			if ((valuePtr = dynamic_cast<Value*>(pr.get())) == nullptr)
				return Value(worker.getNan(), arg->getAct());
			return *valuePtr;
		}

	private:

		Complex _value;

	private:

		const MathType getMT() const override
		{
			return MathBase::MathType::value;
		}

	};

}
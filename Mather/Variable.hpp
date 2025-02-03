#pragma once

#include<string>
#include<memory>

#include"BaseClasses.hpp"
#include"Value.hpp"

namespace expr
{

	template<typename Complex>
	class Variable : public MathBase
	{
	public:
		Variable() : MathBase() {}
		Variable(const Variable& other) : MathBase(other), _name(other._name) {}
		Variable(const ActionT action) : MathBase(action) {}
		Variable(const std::string& name, const ActionT act = ActionT::none) : MathBase(act), _name(name) {}
		
		const std::string toStr() const override
		{
			return _name;
		}
		const std::string toMathJaxStr() const override
		{
			return _name;
		}
		
		MathBasePtr getProcessedValue() const override
		{
			mathWorker::MathWorker<Complex> worker;
			return std::make_unique<Value<Complex>>(Value<Complex>(worker.getNan()));
		}
		MathBasePtr copy() const override
		{
			return std::make_unique<Variable>(*this);
		}

		const std::string& getName() const
		{
			return _name;
		}
		void setName(const std::string& name)
		{
			_name = name;
		}

	private:

		std::string _name;

	private:

		const MathType getMT() const override
		{
			return MathBase::MathType::variable;
		}

	};


}
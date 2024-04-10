#include<iostream>
#include<vector>
#include<cmath>

#include<SFML\Network.hpp>
#include<SFML\Graphics.hpp>
#include<myio/out.hpp>

#define PI_val 3.1415926535897932l

using namespace out;

using FType = double;


enum class Action : char
{
	none = 0,		//
	plus = '+',		// +
	minus = '-',	// -
	div = '/',		// -
	rdiv = '%',		// %
	mult = '*',		// *
	pow = '^',		// ^
	log,			// log
	nrt,			// nrt
	idiv			// //
};
enum class FunctionType : char
{
	none,
	sqrt,
	ln,
	lg,
	sin,
	cos,
	tg,
	ctg,
	arcsin,
	arccos,
	arctg,
	arcctg,
	fact
};
enum class Brackets : char
{
	none,
	def,
	round,
	frac,
	modul
};

bool firstAction(const Action a)
{
	return a == Action::pow || a == Action::log || a == Action::nrt;
}
bool secondAction(const Action a)
{
	return a == Action::mult || a == Action::div || a == Action::rdiv || a == Action::idiv;
}
bool thirdAction(const Action a)
{
	return a == Action::plus || a == Action::minus;
}

void outAct(const Action a)
{
	switch (a)
	{
	case Action::log:
		std::cout << "log";
		break;
	case Action::nrt:
		std::cout << "nrt";
		break;
	case Action::idiv:
		std::cout << "//";
		break;
	default:
		std::cout << static_cast<char>(a);
		break;
	}
}
void outFunc(const FunctionType a)
{
	switch (a)
	{
	case FunctionType::sqrt:
		std::cout << "sqrt";
		break;
	case FunctionType::ln:
		std::cout << "ln";
		break;
	case FunctionType::lg:
		std::cout << "lg";
		break;
	case FunctionType::sin:
		std::cout << "sin";
		break;
	case FunctionType::cos:
		std::cout << "cos";
		break;
	case FunctionType::tg:
		std::cout << "tg";
		break;
	case FunctionType::ctg:
		std::cout << "ctg";
		break;
	case FunctionType::arcsin:
		std::cout << "arcsin";
		break;
	case FunctionType::arccos:
		std::cout << "arccos";
		break;
	case FunctionType::arctg:
		std::cout << "arctg";
		break;
	case FunctionType::arcctg:
		std::cout << "arcctg";
		break;
	case FunctionType::fact:
		std::cout << "!";
		break;
	default:
		break;
	}
}
void outOpenBrack(const Brackets a)
{
	switch (a)
	{
	case Brackets::def:
		std::cout << "(";
		break;
	case Brackets::round:
		std::cout << "[";
		break;
	case Brackets::frac:
		std::cout << "{";
		break;
	case Brackets::modul:
		std::cout << "|";
		break;
	default:
		break;
	}
}
void outCloseBrack(const Brackets a)
{
	switch (a)
	{
	case Brackets::def:
		std::cout << ")";
		break;
	case Brackets::round:
		std::cout << "]";
		break;
	case Brackets::frac:
		std::cout << "}";
		break;
	case Brackets::modul:
		std::cout << "|";
		break;
	default:
		break;
	}
}

class ExpressionBase
{
public:
	friend class Value;
	friend class Function;
	friend class Expression;
public:
	ExpressionBase() = default;
	ExpressionBase(const Action& action) : next(action) {};

	virtual std::unique_ptr<ExpressionBase> procession() = 0;

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

	std::unique_ptr<ExpressionBase> procession() override
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
		outAct(next);
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

	std::unique_ptr<ExpressionBase> procession() override
	{
		if (arg == nullptr)
			return std::make_unique<Value>(Value(0));

		std::unique_ptr<ExpressionBase> pr = arg->procession();
		Value* valuePtr = nullptr;
		if ((valuePtr = dynamic_cast<Value*>(pr.get())) == nullptr)
			return std::make_unique<Value>(Value(0));

		return std::make_unique<Value>(Value(processingFunction(valuePtr->getValue())));
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
		outFunc(type);
		std::cout << '(';
		if (arg != nullptr)
			arg->print();
		std::cout << ')';
		outAct(next);
	}

private:

	FunctionType type = FunctionType::none;

	std::unique_ptr<ExpressionBase> arg;

	std::unique_ptr<ExpressionBase> copy() const override
	{
		return std::make_unique<Function>(*this);
	}

	const FType processingFunction(const FType& v)
	{
		switch (type)
		{
		case FunctionType::sqrt:
			return std::sqrt(v);
		case FunctionType::ln:
			return std::log(v);
		case FunctionType::lg:
			return std::log(v) / std::log(10);
		case FunctionType::sin:
			return std::sin(v);
		case FunctionType::cos:
			return std::cos(v);
		case FunctionType::tg:
			return std::tan(v);
		case FunctionType::ctg:
			return 1 / std::tan(v);
		case FunctionType::arcsin:
			return std::asin(v);
		case FunctionType::arccos:
			return std::acos(v);
		case FunctionType::arctg:
			return std::atan(v);
		case FunctionType::arcctg:
			return PI_val / 2 - std::atan(v);
		case FunctionType::fact:
			return std::tgamma(v);
		default:
			return 0;
		}
	}

};

class Expression : public ExpressionBase
{
public:
	//friend class ExpressionProcesser;
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

	std::unique_ptr<ExpressionBase> procession()
	{
		deleteNullptr();

		for (size_t i = 0; i < values.size(); ++i)
			values[i] = std::move(values[i]->procession());

		processActions();


		return std::make_unique<Expression>(*this);
	}

	void add(const ExpressionBase& expr)
	{
		values.push_back(std::move(expr.copy()));
	}

	void print() const override
	{
		outOpenBrack(brk);
		for (const auto& i : values)
			i->print();
		outCloseBrack(brk);
	}

private:
	Brackets brk = Brackets::none;

	std::vector<std::unique_ptr<ExpressionBase>> values;

	std::unique_ptr<ExpressionBase> copy() const override
	{
		return std::make_unique<Expression>(*this);
	}

	void deleteNullptr()
	{
		for (auto ptr = values.begin(); ptr != values.end();)
			if (*ptr == nullptr)
				ptr = values.erase(ptr);
			else
				++ptr;
	}

	void processActions()
	{
		processFirst();
		processSecond();
		processThird();
	}

	void processFirst()
	{
		for (size_t i = 0; i < values.size() - 1; ++i)
		{
			if (firstAction(values[i]->getNextAction()))
			{
				Value* ptr1 = dynamic_cast<Value*>(values[i].get());
				Value* ptr2 = dynamic_cast<Value*>(values[i + 1].get());
				Value res;
				if (ptr1 != nullptr && ptr2 != nullptr)
				{
					if (ptr1->getNextAction() == Action::pow)
						res.setValue(std::pow(ptr1->getValue(), ptr2->getValue()));
					else if (ptr1->getNextAction() == Action::nrt)
						res.setValue(std::pow(ptr1->getValue(), 1 / ptr2->getValue()));
					else if (ptr1->getNextAction() == Action::log)
						res.setValue(std::log(ptr1->getValue()) / std::log(ptr2->getValue()));
					res.setNextAction(ptr2->getNextAction());
				}
				values[i] = std::move(std::make_unique<Value>(res));
				values.erase(values.begin() + i + 1);
			}
		}
	}
	void processSecond()
	{
		for (size_t i = 0; i < values.size() - 1; ++i)
		{
			if (secondAction(values[i]->getNextAction()))
			{
				Value* ptr1 = dynamic_cast<Value*>(values[i].get());
				Value* ptr2 = dynamic_cast<Value*>(values[i + 1].get());
				Value res;
				if (ptr1 != nullptr && ptr2 != nullptr)
				{
					if (ptr1->getNextAction() == Action::mult)
						res.setValue(ptr1->getValue() * ptr2->getValue());
					else if (ptr1->getNextAction() == Action::div)
						res.setValue(ptr1->getValue() / ptr2->getValue());
					else if (ptr1->getNextAction() == Action::rdiv)
						res.setValue(std::fmod(ptr1->getValue(), ptr2->getValue()));
					else if (ptr1->getNextAction() == Action::idiv)
						res.setValue(std::round(ptr1->getValue() / ptr2->getValue()));
					res.setNextAction(ptr2->getNextAction());
				}
				values[i] = std::move(std::make_unique<Value>(res));
				values.erase(values.begin() + i + 1);
			}
		}
	}
	void processThird()
	{
		for (size_t i = 0; i < values.size() - 1; ++i)
		{
			if (thirdAction(values[i]->getNextAction()))
			{
				Value* ptr1 = dynamic_cast<Value*>(values[i].get());
				Value* ptr2 = dynamic_cast<Value*>(values[i + 1].get());
				Value res;
				if (ptr1 != nullptr && ptr2 != nullptr)
				{
					if (ptr1->getNextAction() == Action::plus)
						res.setValue(ptr1->getValue() + ptr2->getValue());
					else if (ptr1->getNextAction() == Action::minus)
						res.setValue(ptr1->getValue() - ptr2->getValue());
					res.setNextAction(ptr2->getNextAction());
				}
				values[i] = std::move(std::make_unique<Value>(res));
				values.erase(values.begin() + i + 1);
			}
		}
	}


	

};

int main()
{
	system("chcp 65001 > null");
	
	Value pr;
	Expression ex;

	pr = 156;
	pr.setNextAction(Action::div);
	ex.add(pr);
	
	pr = 12;
	pr.setNextAction(Action::plus);
	ex.add(pr);
	
	pr = -7.5;
	pr.setNextAction(Action::rdiv);
	ex.add(pr);
	
	pr = 0.5;
	pr.setNextAction(Action::none);
	ex.add(pr);







	ex.print();
	std::cout << '\n';
	auto ptr = ex.procession();
	ptr->print();



	return 0;
}


////Сервер
//sf::TcpListener listener;
//sf::TcpSocket client;
//
//void sendMessage()
//{
//	std::string str;
//	for (int i = 0; i < 10; ++i)
//	{
//		std::getline(std::cin, str);
//		while (client.send(str.c_str(), str.size() + 1) != sf::Socket::Done) {}
//	}
//}
//
//void getMessage()
//{
//	char bytes[128]{};
//	size_t received;
//	for (int i = 0; i < 10; ++i)
//	{
//		while (client.receive(bytes, 128, received) != sf::Socket::Done) {}
//			print(bytes, '\n');
//	}
//}
//
//int main()
//{
//	system("chcp 1251 > nul");
//	
//	if (listener.listen(53000) != sf::Socket::Done)
//		print("Ошабка прослушивания\n");
//
//	if (listener.accept(client) != sf::Socket::Done)
//		print("Ошибка привязки клиента\n");
//	else
//		print("Привязка клиента успеша!\n");
//
//	std::thread set(sendMessage);
//	std::thread get(getMessage);
//
//	set.join();
//	get.join();
//
//	return 0;
//}




//#define IP_SERVER_ADRESS "192.168.0.154"
//
//using namespace out;
//
////������
//sf::TcpSocket socket;
//sf::Socket::Status status;
//
//void sendMessage()
//{
//	std::string str;
//	for (int i = 0; i < 10; ++i)
//	{
//		std::getline(std::cin, str);
//		while (socket.send(str.c_str(), str.size() + 1) != sf::Socket::Done) {}
//	}
//}
//
//void getMessage()
//{
//	char bytes[128]{};
//	size_t received;
//	for (int i = 0; i < 10; ++i)
//	{
//		while (socket.receive(bytes, 128, received) != sf::Socket::Done) {}
//		print(bytes, '\n');
//	}
//}
//
//int main()
//{
//	system("chcp 1251 > nul");
//
//	status = socket.connect(IP_SERVER_ADRESS, 53000);
//	if (status != sf::Socket::Done)
//		print("������ �����������\n");
//	else
//		print("����������� �������\n");
//
//	std::thread set(sendMessage);
//	std::thread get(getMessage);
//
//	set.join();
//	get.join();
//
//	return 0;
//}
#include<iostream>

#include<iostream>
#include<vector>
#include<cmath>
#include<memory>
#include <regex>
#include <stack>

#include<myio/out.hpp>
#include<myio/in.hpp>

#define PI_val 3.1415926535897932l

using namespace out;
using namespace in;

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
	def,	//default	()
	round,	//round		[]
	frac,	//fraction	{}
	modul	//modul		||
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

	const FType processingFunction(const FType& v) const
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
		std::vector<Value> vls;
		
		for (const auto& i : values)
			if (i != nullptr)
				vls.push_back(*dynamic_cast<Value*>(i->processed().get()));

		vls.back().setNextAction(Action::none);

		firstProcessed(vls);
		secondProcessed(vls);
		thirdProcessed(vls);

		if (brk == Brackets::round)
			vls[0].setValue(std::round(vls[0].getValue()));
		if (brk == Brackets::frac)
		{
			FType pr;
			vls[0].setValue(std::modf(vls[0].getValue(), &pr));
		}
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
		outOpenBrack(brk);
		for (const auto& i : values)
			i->print();
		outCloseBrack(brk);
		outAct(next);
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
			if (firstAction(vls[i].getNextAction()))
			{
				if (vls[i].getNextAction() == Action::pow)
					pr.setValue(std::pow(vls[i].getValue(), vls[i+1].getValue()));
				else if (vls[i].getNextAction() == Action::nrt)
					pr.setValue(std::pow(vls[i].getValue(), 1 / vls[i + 1].getValue()));
				else if (vls[i].getNextAction() == Action::pow)
					pr.setValue(std::log(vls[i].getValue()) / std::log(vls[i + 1].getValue()));
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
			if (secondAction(vls[i].getNextAction()))
			{
				if (vls[i].getNextAction() == Action::mult)
					pr.setValue(vls[i].getValue() * vls[i+1].getValue());
				else if (vls[i].getNextAction() == Action::div)
					pr.setValue(vls[i].getValue() / vls[i + 1].getValue());
				else if (vls[i].getNextAction() == Action::rdiv)
					pr.setValue(std::fmod(vls[i].getValue(), vls[i + 1].getValue()));
				else if (vls[i].getNextAction() == Action::idiv)
					pr.setValue(std::round(vls[i].getValue() /  vls[i + 1].getValue()));

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
			if (thirdAction(vls[i].getNextAction()))
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

class ExpressionParser
{
public:

	Expression parse(std::string str)
	{
		modulEdit(str);
		std::vector<std::string> tokens;
		tokenize(str, tokens);

		Expression res;

		for (size_t i = 0; i < tokens.size(); ++i)
		{
			if (isNum(tokens[i]))
			{
				Value prv = std::stold(tokens[i]);
				if (i != tokens.size() - 1)
					prv.setNextAction(parseAction(tokens[i++ + 1]));
				res.add(prv);
			}
			else if (isFunc(tokens[i]))
			{
				Function prf;
				prf.setType(parseFunction(tokens[i]));
				prf.setArg(subParse(tokens[i + 1]));
				if (i + 1 != tokens.size() - 1)
					prf.setNextAction(parseAction(tokens[i++ + 2]));
				res.add(prf);
				i++;
			}
			else if (isOpenBracket(tokens[i][0]))
			{
				Expression pre = subParse(tokens[i]);
				if (i != tokens.size() - 1)
					pre.setNextAction(parseAction(tokens[i++ + 1]));
				res.add(pre);
			}
		}


		return res; return subParse(str);
	}

private:

	Expression subParse(std::string str)
	{
		Brackets brk = parseBracket(str[0]);
		if (brk != Brackets::none)
		{
			str.pop_back();
			str.erase(str.begin());
		}

		std::vector<std::string> tokens;
		tokenize(str, tokens);
	
		Expression res;
		res.setBrackets(brk);
	
		for (size_t i = 0; i < tokens.size(); ++i)
		{
			if (isNum(tokens[i]))
			{
				Value prv = std::stold(tokens[i]);
				if (i != tokens.size() - 1)
					prv.setNextAction(parseAction(tokens[i++ + 1]));
				res.add(prv);
			}
			else if(isFunc(tokens[i]))
			{
				Function prf;
				prf.setType(parseFunction(tokens[i]));
				prf.setArg(subParse(tokens[i + 1]));
				if (i + 1 != tokens.size() - 1)
					prf.setNextAction(parseAction(tokens[i++ + 2]));
				res.add(prf);
				i++;
			}
			else if (isOpenBracket(tokens[i][0]))
			{
				Expression pre = subParse(tokens[i]);
				if(i != tokens.size() - 1)
					pre.setNextAction(parseAction(tokens[i++ + 1]));
				res.add(pre);
			}
		}


		return res;
	}

private:

	void modulEdit(std::string& str)
	{
		if (str[0] == '|')
			str[0] = '<';
		size_t pos;
		while ((pos = str.find('|')) != std::string::npos)
		{
			if (!(str[pos - 1] >= '0' && str[pos - 1] <= '9') && str[pos - 1] != ')' && str[pos - 1] != ']' && str[pos - 1] != '}' && str[pos - 1] != '|')
				str[pos] = '<';
			else
				str[pos] = '>';
		}
	}

	void tokenize(const std::string& expression, std::vector<std::string>& tokens)
	{
		std::regex pattern("([a-zA-Z]+|\\d+(\\.\\d+)?|//|<[^>]+>|\\([^\\)]+\\)|\\[[^\\]]+\\]|\\{[^\\}]+\\}|[^a-zA-Z0-9])");
		std::sregex_iterator words_begin = std::sregex_iterator(expression.begin(), expression.end(), pattern);
		std::sregex_iterator words_end = std::sregex_iterator();
		for (std::sregex_iterator i = words_begin; i != words_end; ++i)
		{
			std::smatch match = *i;
			for (size_t j = 1; j < match.size(); ++j)
			{
				if (!match[j].str().empty())
				{
					tokens.push_back(match[j].str());
					break;
				}
			}
		}
	}

private:

	Action parseAction(const std::string& s)
	{
		switch (s[0])
		{
		case '+':
			return Action::plus;
		case '-':
			return Action::minus;
		case '*':
			return Action::mult;
		case '/':
			return Action::div;
		case '%':
			return Action::rdiv;
		case '^':
			return Action::pow;
		default:
			if(s == "//")
				return Action::idiv;
			if(s == "log")
				return Action::log;
			if(s == "nrt")
				return Action::nrt;
			return Action::none;
		}
	}

	FunctionType parseFunction(const std::string& s)
	{
		if (s == "sqrt")
			return FunctionType::sqrt;
		if (s == "ln")
			return FunctionType::ln;
		if (s == "lg")
			return FunctionType::lg;
		if (s == "sin")
			return FunctionType::sin;
		if (s == "cos")
			return FunctionType::cos;
		if (s == "tg")
			return FunctionType::tg;
		if (s == "ctg")
			return FunctionType::ctg;
		if (s == "arcsin")
			return FunctionType::arcsin;
		if (s == "arccos")
			return FunctionType::arccos;
		if (s == "arctg")
			return FunctionType::arctg;
		if (s == "arcctg")
			return FunctionType::arcctg;
		if (s == "fact")
			return FunctionType::fact;
		return FunctionType::none;
	}

	Brackets parseBracket(const char c)
	{
		if (c == '(')
			return Brackets::def;
		if (c == '[')
			return Brackets::round;
		if (c == '|')
			return Brackets::modul;
		if (c == '{')
			return Brackets::frac;
		return Brackets::none;
	}

private:

	bool isNum(const std::string& str)
	{
		return str[0] >= '0' && str[0] <= '9';
	}
	bool isFunc(const std::string& str)
	{
		return str[0] >= 'a' && str[0] <= 'z';
	}
	bool isOpenBracket(const char c)
	{
		return c == '(' || c == '{' || c == '[' || c == '<';
	}

	char oppositeBracket(const char c)
	{
		switch (c)
		{
		case '(': return ')';
		case ')': return '(';
		case '[': return ']';
		case ']': return '[';
		case '{': return '}';
		case '}': return '{';
		case '<': return '>';
		case '>': return '<';
		default: return '\0';
		}
	}

	bool goodBrackets(const std::vector<std::string>& arr)
	{
		std::stack<char> st;
		for (const auto& i : arr)
		{
			if (isOpenBracket(i[0]))
			{
				if (st.top() == oppositeBracket(i[0]))
					st.pop();
				else
					st.push(i[0]);

			}
		}
		return st.empty();
	}

};

int main()
{
	system("chcp 65001 > nul");

	ExpressionParser parser;

	while (true)
	{
		print(parser.parse(input()).getProcessingResult().getValue());
	}

	//Expression exp = parser.parse("(2+2)*2+sin(3.1415926/2)-[3.1415926]");
	//exp.print();
	//print(end, exp.getProcessingResult().getValue());

	//print(tokenize("(2+2)*2"));
	//print(tokenize("132+3*arccos(15.5)-999//35/9-<22-99>-(29+9)"));
	//print(tokenize("2*(2+2)+sin(39+4!)"));
	//print(tokenize("-<ab>ggg"));
	//print(tokenize("+log15(25)"));


	//Value v1;
	//Value v2;
	//Expression e1;

	//v2 = 2;
	//v2.setNextAction(Action::plus);

	//v1.setNextAction(Action::pow);

	//v1 = 10;
	//e1.add(v1);
	//e1.add(v2);

	//v1 = 11;
	//e1.add(v1);
	//e1.add(v2);

	//v1 = 12;
	//e1.add(v1);
	//e1.add(v2);

	//v1 = 13;
	//e1.add(v1);
	//e1.add(v2);

	//v1 = 14;
	//e1.add(v1);
	//e1.add(v2);

	//e1.setBrackets(Brackets::def);

	//Expression res;

	//e1.setNextAction(Action::div);

	//res.add(e1);

	//res.add(Value(365));

	//res.print();

	//Value d = res.getProcessingResult();
	//std::cout << '\n' << d.getValue() << '\n';

	return 0;
}
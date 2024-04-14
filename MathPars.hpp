#pragma once

#include<stack>
#include<regex>

#include"MathExpressionErrors.hpp"

#include"MathExpression.hpp"

namespace expr
{
	class Parser
	{
	public:

		Expression parse(std::string& str)
		{
			modulEdit(str);
			goodBrackets(str);
			constantEdit(str);
			std::vector<std::string> tkns;
			tokenize(str, tkns);
			Expression res;
			fillExpression(tkns, res);
			return res;
		}

	private:

		void strParse(std::string str, Expression& res)
		{
			if (Brackets brk = parseBracket(str[0]); brk != Brackets::none)
			{
				res.setBrackets(brk);
				str.pop_back();
				str.erase(str.begin());
			}

			std::vector<std::string> tkns;
			tokenize(str, tkns);

			fillExpression(tkns, res);
		}

		void fillExpression(const std::vector<std::string>& tkns, Expression& expr)
		{
			for (size_t i = 0; i < tkns.size(); ++i)
			{
				if (isNum(tkns[i]))
				{
					Value prv = toFType(tkns[i]);
					fillAction(expr, prv, tkns, i);
				}
				else if (isWord(tkns[i]))
				{
					if (fillSpecialWords(tkns, i, expr))
						continue;
					Function prf;
					prf.setType(parseFunction(tkns[i++]));
					Expression pre;
					strParse(tkns[i], pre);
					prf.setArg(pre);
					fillAction(expr, prf, tkns, i);
				}
				else if (parseBracket(tkns[i][0]) != Brackets::none)
				{
					Expression pre;
					strParse(tkns[i], pre);
					fillAction(expr, pre, tkns, i);
				}
				else if (tkns[i][0] == '-')
				{
					Value prv = FType(-1);
					prv.setNextAction(Action::mult);
					expr.add(prv);
				}
			}
		}

		void fillAction(Expression& res, ExpressionBase& value, const std::vector<std::string>& tkns, size_t& i)
		{
			if (i + 1 != tkns.size())
			{
				if (specialFunc(tkns[i + 1][0]))
				{
					Function prf;
					prf.setType(parseSpecialType(tkns[i + 1][0]));
					prf.setArg(value);
					i++;
					if (i + 1 != tkns.size())
					{
						Action act = act = parseAction(tkns[i + 1]);
						if (act == Action::none)
							prf.setNextAction(Action::mult);
						else
							prf.setNextAction((i++, act));
					}
					res.add(prf);
					return;
				}
				else
				{
					Action act = act = parseAction(tkns[i + 1]);
					if (act == Action::none)
						value.setNextAction(Action::mult);
					else
						value.setNextAction((i++, act));
				}
			}
			res.add(value);
		}

		bool fillSpecialWords(const std::vector<std::string>& tkns, size_t& i, Expression& expr)
		{
			if (tkns[i] == "log")
			{
				Expression pre1, pre2;
				strParse(tkns[++i], pre1);
				strParse(tkns[++i], pre2);
				pre1.setNextAction(Action::log);
				expr.add(pre1);
				fillAction(expr, pre2, tkns, i);
				return true;
			}
			else if (tkns[i] == "inf")
			{
				Value prv = FType(1.l / std::sin(0.l));
				fillAction(expr, prv, tkns, i);
				return true;
			}
			return false;
		}

	private://str edit

		void modulEdit(std::string& str)
		{
			if (str[0] == '|')
				str[0] = '<';
			size_t pos;
			while ((pos = str.find('|')) != std::string::npos)
			{
				if (!(str[pos - 1] >= '0' && str[pos - 1] <= '9') && !isCloseBracket(str[pos - 1]) && str[pos - 1] != 'i' && str[pos - 1] != '|' && !specialFunc(str[pos - 1]))
					str[pos] = '<';
				else
					str[pos] = '>';
			}
		}

		void constantEdit(std::string& str)
		{
			replaceAll(str, "pi", "3.1415926535897932");
			replaceAll(str, "e", "2.7182818284590452");
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
			if (tokens[0][0] == 'i')
				tokens[0] = "1i";
			for (size_t i = tokens.size() - 1; i > 0; --i)
			{
				if (isCloseBracket(tokens[i][0]))
				{
					tokens[i - 1] += tokens[i];
					tokens.erase(tokens.begin() + i);
				}
				else if (tokens[i][0] == 'i')
				{
					if (isNum(tokens[i - 1]))
					{
						tokens[i - 1].push_back('i');
						tokens.erase(tokens.begin() + i);
					}
					else
						tokens[i] = "1i";
				}
			}
		}

	private://enum parse

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
			case '%':
				return Action::rdiv;
			case '^':
				return Action::pow;
			default:
				if (s == "//")
					return Action::idiv;
				if (s == "/")
					return Action::div;
				if (s == "nrt")
					return Action::nrt;
				return Action::none;
			}
		}
		Brackets parseBracket(const char c)
		{
			switch (c)
			{
			case '(':
			case ')':
				return Brackets::def;
			case '[':
			case ']':
				return Brackets::round;
			case '{':
			case '}':
				return Brackets::frac;
			case '<':
			case '>':
				return Brackets::modul;
			default:
				return Brackets::none;
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
			if (s == "d")
				return FunctionType::degrees;
			return FunctionType::none;
		}
		FunctionType parseSpecialType(const char c)
		{
			if (c == '!')
				return FunctionType::fact;
			if (c == 'd')
				return FunctionType::degrees;
			if (c == 'r')
				return FunctionType::radian;
			return FunctionType::none;
		}

	private://special func

		bool specialFunc(const char c)
		{
			return c == 'd' || c == '!' || c == 'r';
		}
		bool isOpenBracket(const char c)
		{
			return c == '(' || c == '{' || c == '[' || c == '<';
		}
		bool isCloseBracket(const char c)
		{
			return c == ')' || c == '}' || c == ']' || c == '>';
		}
		bool isNum(const std::string& str)
		{
			return str[0] >= '0' && str[0] <= '9';
		}
		bool isWord(const std::string& str)
		{
			return str[0] >= 'a' && str[0] <= 'z';
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
		void goodBrackets(const std::string& str)
		{
			std::stack<char> st;
			for (size_t i = 0; i < str.size(); ++i)
				if (isOpenBracket(str[i]))
					st.push(str[i]);
				else if (isCloseBracket(str[i]))
				{
					if (st.top() != oppositeBracket(str[i]))
						throw ParseException("Bracket " + std::string(1, str[i]) + std::string(" not correct closed at position: " + std::to_string(i)), ParseException::ErrorType::brackets);
					st.pop();
				}
			if (!st.empty())
				throw ParseException("Brackets " + std::string(1, st.top()) + " is not closed", {ParseException::ErrorType::brackets});
		}

	};
}
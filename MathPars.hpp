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

		Expression parse(std::string str) throw(ParseException)
		{
			modulEdit(str);
			if (!goodBrackets(str))
				throw ParseException("Brackets error.", ParseException::ErrorType::brackets);
			constantEdit(str);
			std::vector<std::string> tkns;
			tokenize(str, tkns);
			Expression res;
			fillExpression(tkns, res);
			return res;
		}

	private://str processing

		void strParse(std::string str, Expression& res)
		{
			if (Brackets brk = enumFunc::parseBracket(str[0]); brk != Brackets::none)
			{
				if (str.size() < 3)
					throw ParseException("Empty brackets.", ParseException::ErrorType::brackets);
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
				if (enumFunc::isNum(tkns[i]))
				{
					Value prv = myMath::toFType(tkns[i]);
					fillAction(tkns, i, expr, prv);
				}
				else if (enumFunc::isWord(tkns[i]))
					fillByWord(tkns, i, expr);
				else if (enumFunc::parseBracket(tkns[i][0]) != Brackets::none)
				{
					Expression pre;
					strParse(tkns[i], pre);
					fillAction(tkns, i, expr, pre);
				}
				else if (tkns[i][0] == '-')
				{
					Value prv = FType(-1);
					prv.setNextAction(Action::mult);
					expr.add(prv);
				}
				else if (enumFunc::parseAction(tkns[i]) == Action::none)
					throw ParseException("Unknown word: \"" + tkns[i] + "\"", ParseException::ErrorType::word);
			}
		}

	private://sub fill strs

		void fillByWord(const std::vector<std::string>& tkns, size_t& i, Expression& expr)
		{
			if (fillSpecialWords(tkns, i, expr))
				return;
			Function prf;
			prf.setType(enumFunc::parseFunction(tkns[i++]));
			if (prf.getType() == FunctionType::none)
				throw ParseException("Outside unknown word \"" + tkns[i - 1] + '\"', ParseException::ErrorType::func);
			if (i == tkns.size())
				throw ParseException("Outside argument for function \"" + tkns[i - 1] + '\"', ParseException::ErrorType::func);
			if (enumFunc::isNum(tkns[i]))
				prf.setArg(Value(myMath::toFType(tkns[i])));
			else if (enumFunc::isOpenBracket(tkns[i][0]))
			{
				Expression pre;
				strParse(tkns[i], pre);
				prf.setArg(pre);
			}
			else
				throw ParseException("Bad function argument: \"" + tkns[i] + '\"', ParseException::ErrorType::func);
			fillAction(tkns, i, expr, prf);
		}

		bool fillSpecialWords(const std::vector<std::string>& tkns, size_t& i, Expression& expr)
		{
			if (tkns[i] == "log")
			{
				Expression pre1, pre2;
				if (i + 2 >= tkns.size())
					throw ParseException("The log function without arguments.", ParseException::ErrorType::func);
				if (!((enumFunc::isNum(tkns[i + 1]) || enumFunc::isOpenBracket(tkns[i + 1][0])) && (enumFunc::isNum(tkns[i + 2]) || enumFunc::isOpenBracket(tkns[i + 2][0]))))
					throw ParseException("The log argument is not a number.", ParseException::ErrorType::func);
				strParse(tkns[++i], pre1);
				strParse(tkns[++i], pre2);
				pre1.setNextAction(Action::log);
				expr.add(pre1);
				fillAction(tkns, i, expr, pre2);
				return true;
			}
			else if (tkns[i] == "inf")
			{
				Value prv = FType(1.l / std::sin(0.l));
				fillAction(tkns, i, expr, prv);
				return true;
			}
			return false;
		}

		void fillAction(const std::vector<std::string>& tkns, size_t& i, Expression& expr, ExpressionBase& value)
		{
			if (i + 1 != tkns.size())
			{
				if (enumFunc::specialFunc(tkns[i + 1][0]))
				{
					Function prf;
					prf.setType(enumFunc::parseSpecialType(tkns[i + 1][0]));
					prf.setArg(value);
					i++;
					if (i + 1 != tkns.size())
					{
						Action act = act = enumFunc::parseAction(tkns[i + 1]);
						if (act == Action::none)
							prf.setNextAction(Action::mult);
						else
							prf.setNextAction((i++, act));
					}
					expr.add(prf);
					return;
				}
				else
				{
					Action act = act = enumFunc::parseAction(tkns[i + 1]);
					if (act == Action::none)
						value.setNextAction(Action::mult);
					else
						value.setNextAction((i++, act));
				}
			}
			expr.add(value);
		}

	private://str edit

		void modulEdit(std::string& str)
		{
			if (str[0] == '|')
				str[0] = '<';
			size_t pos;
			while ((pos = str.find('|')) != std::string::npos)
			{
				if (!(str[pos - 1] >= '0' && str[pos - 1] <= '9') && !enumFunc::isCloseBracket(str[pos - 1]) && str[pos - 1] != 'i' && str[pos - 1] != '|' && !enumFunc::specialFunc(str[pos - 1]))
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
				if ((enumFunc::isOpenBracket(tokens[i][0]) || enumFunc::isCloseBracket(tokens[i].back())) && !goodBrackets(tokens[i]))
				{
					tokens[i - 1] += tokens[i];
					tokens.erase(tokens.begin() + i);
				}
				else if (tokens[i].size() == 1 && tokens[i][0] == 'i')
				{
					if (enumFunc::isNum(tokens[i - 1]))
					{
						tokens[i - 1].push_back('i');
						tokens.erase(tokens.begin() + i);
					}
					else
						tokens[i] = "1i";
				}
			}
		}

		bool goodBrackets(const std::string& str)
		{
			std::stack<char> s;
			for (const auto& i : str)
				if (enumFunc::isOpenBracket(i))
					s.push(i);
				else if (enumFunc::isCloseBracket(i))
				{
					if (s.empty() || s.top() != enumFunc::oppositeBracket(i))
						return false;
					s.pop();
				}
			return s.empty();
		}

	};
}
#pragma once

#include <stack>
#include <regex>

#include"../Mather/Value.hpp"
#include"../Mather/StandartFunction.hpp"
#include"../Mather/TwoPramFunction.hpp"
#include"../Mather/PostfixFunction.hpp"
#include"../Mather/Expression.hpp"

#include "ParserException.hpp"

namespace expr
{
	namespace parse
	{
		bool isGoodBrackets(const std::string& str)
		{
			std::stack<char> s;
			for (const auto& i : str)
				if (i == '(')
					s.push(i);
				else if (i == ')')
				{
					if (s.empty() || s.top() != mconverter.oppositeBracket(i))
						return false;
					s.pop();
				}
			return s.empty();
		}

		class Tokenizer
		{
		public:

			void tokenizer(const std::string& expression, std::vector<std::string>& tokens) const
			{
				std::regex pattern(
					"([a-zA-Z]+|\\d+(\\.\\d+)?|//"
					"|<[^>]+>|\\([^\\)]+\\)|\\[[^\\]]+\\]|\\{[^\\}]+\\}|[^a-zA-Z0-9])");
				std::sregex_iterator words_begin =
					std::sregex_iterator(expression.begin(), expression.end(), pattern);
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
				for (size_t i = tokens.size() - 1; i > 0; --i)
				{
					if ((tokens[i][0] == '(' || tokens[i].back() == ')') && !isGoodBrackets(tokens[i]))
					{
						tokens[i - 1] += tokens[i];
						tokens.erase(tokens.begin() + i);
					}
				}
			}

			void tokinizerPolinom(const std::string& expression, std::vector<std::string>& tokens) const
			{
				tokens.clear();
				std::regex pattern("([a-zA-Z]|\\d+(\\.\\d+)?|\\(|\\)|[^a-zA-Z0-9()])");
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
				for (size_t i = tokens.size() - 1; i > 0; --i)
				{
					if ((tokens[i][0] == '(' || tokens[i].back() == ')') && !isGoodBrackets(tokens[i]))
					{
						tokens[i - 1] += tokens[i];
						tokens.erase(tokens.begin() + i);
					}
				}
			}

			void tokenizerEquation(const std::string& expression, std::vector<std::string>& tokens) const
			{
				tokens.clear();
				size_t last = 0;
				for (size_t ind = expression.find('x'); ind != std::string::npos; last = ind, ind = expression.find('x', ind))
				{
					++ind;
					if (expression[ind] != '^')
					{
						tokens.push_back(expression.substr(last, ind - last));
						continue;
					}
					while (mconverter.isNum(expression[++ind])) {}
					tokens.push_back(expression.substr(last, ind - last));
				}
				tokens.push_back(expression.substr(last));
			}

		};

	}
}
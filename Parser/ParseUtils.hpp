#pragma once

#include <regex>
#include <stack>

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
		namespace parseFuncs
		{
			bool isOpenBracket(const char c)
			{
				return c == '(' || c == '[' || c == '{' || c == '<';
			}
			bool isCloseBracket(const char c)
			{
				return c == ')' || c == '}' || c == '>' || c == ']';
			}
			bool isWord(const char c)
			{
				return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
			}
			bool isNum(const char c)
			{
				return (c >= '0' && c <= '9') || c == 'i';
			}
			bool isNum(const std::string& s)
			{
				return isNum(s[0]) || (s.size() >= 2 && (s[0] == '-' || s[0] == '.') && isNum(s[1])) || s == "nan" || s == "-nan" || s == "inf" || s == "-inf";
			}
		}

		template<typename Complex>
		class Preprocessor
		{
		public:

			void fullPreprocess(std::string& expression) const
			{
				constantEdding(mconverter.getConstantDictionary(), expression);
				modulEdit(expression);
			}

			void modulEdit(std::string& str) const
			{
				if (str[0] == '|')
					str[0] = '<';
				if (str.back() == '|')
					str.back() = '>';

				size_t pos;
				while ((pos = str.find('|')) != std::string::npos)
				{
					if (parseFuncs::isNum(str[pos - 1]) || parseFuncs::isCloseBracket(str[pos - 1]))
						str[pos] = '>';
					else
						str[pos] = '<';
				}
			}

			void constantEdding(const std::map<std::string, std::string>& map, std::string& str) const
			{
				for (auto const& [key, value] : map)
					replaceAll(str, key, value);
			}

			bool isGoodBrackets(const std::string& str) const
			{
				std::stack<char> s;
				for (const auto& i : str)
					if (parseFuncs::isOpenBracket(i))
						s.push(i);
					else if (parseFuncs::isCloseBracket(i))
					{
						if (s.empty() || s.top() != mconverter.oppositeBracket(i))
							return false;
						s.pop();
					}
				return s.empty();
			}

		};

		template<typename Complex>
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
				Preprocessor<Complex> proc;
				for (size_t i = tokens.size() - 1; i > 0; --i)
				{
					if ((parseFuncs::isOpenBracket(tokens[i][0]) || parseFuncs::isCloseBracket(tokens[i].back())) &&
						!proc.isGoodBrackets(tokens[i]))
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
					while (parseFuncs::isNum(expression[++ind])) {}
					tokens.push_back(expression.substr(last, ind - last));
				}
				tokens.push_back(expression.substr(last));
			}
		};

	}
}
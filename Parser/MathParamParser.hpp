#pragma once

#include "ParseUtils.hpp"
#include "MathParser.hpp"

namespace expr
{
	namespace parse
	{

		template<typename Complex>
		class MathParamParser
		{
		public:

			Value<Complex> parse(std::string str) const
			{
				preproc(str);
				std::map<std::string, std::string> variabels;
				fillVariables(variabels, str);
				solveVariables(variabels);
				insertVariables(variabels, str);

				return Value<Complex>::convertProcessed(parser.parse(str));
			}
			Expression<Complex> parseToExpression(std::string str)
			{
				preproc(str);
				std::map<std::string, std::string> variabels;
				fillVariables(variabels, str);
				solveVariables(variabels);
				insertVariables(variabels, str);

				return parser.parse(str);
			}

			const std::string parseNoExcept(std::string str) const noexcept
			{
				try
				{
					return parse(str).toStr();
				}
				catch (const ParseException& e)
				{
					return e.swhat();
				}
				catch (...)
				{
					return "Unknown error!";
				}
			}

		private:

			MathParser<Complex> parser;

		private:

			void preproc(std::string& str) const
			{
				for (size_t i = 0; i < str.size(); ++i)
					if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
					{
						str.erase(str.begin() + i);
						--i;
					}
			}
			void fillVariables(std::map<std::string, std::string>& vars, std::string& str) const
			{
				size_t dotInd = str.find(',');
				if (dotInd == std::string::npos)
					return;
				size_t ind, begin = 0, end = 0;
				do
				{
					ind = str.find(',', end);
					begin = str.find('=', ind);
					end = std::min(str.find(',', begin), str.size());
					if (ind == std::string::npos || begin == std::string::npos || end == std::string::npos)
						break;
					vars.insert(std::make_pair(str.substr(ind + 1, begin - ind - 1), str.substr(begin + 1, end - begin - 1)));
				} while (end != std::string::npos);
				str.erase(dotInd, str.size() - dotInd);
			}
			void solveVariables(std::map<std::string, std::string>& vars) const
			{
				for (auto& [key, value] : vars)
				{
					Value<Complex> v = Value<Complex>::convertProcessed(parser.parse(value));
					value = '(' + v.toStr() + ')';
				}
			}
			void insertVariables(const std::map<std::string, std::string>& vars, std::string& str) const
			{
				Preprocessor<Complex> proc;
				proc.constantEdding(vars, str);
			}
		};

	}

}
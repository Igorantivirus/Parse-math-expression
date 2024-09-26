#pragma once

#include "../Mather/Expression.hpp"
#include "MathParser.hpp"

namespace expr
{

	namespace parse
	{

		template<typename Complex>
		class EquationParser
		{
		public:

			void parse(const std::string& str, std::vector<Expression<Complex>>& res) const
			{
				res.clear();
				Tokenizer<Complex> tokniser;

				std::vector<std::string> tkns;
				tokniser.tokenizerEquation(str, tkns);
				res.resize(1);

				for (const auto& i : tkns)
					processAt(i, res);
			}


		private:

			MathParser<Complex> parser;

		private:

			void processAt(std::string str, std::vector<Expression<Complex>>& res) const
			{
				if (!str.empty() && str[0] == '+')
					str.erase(str.begin());
				size_t ind = str.find('x');
				if (ind == std::string::npos)
				{
					res.back() = parser.parse(str.substr(0, ind));
					return;
				}
				if (ind + 1 == str.size())
				{
					resizeRes(res, 1);
					res[res.size() - 2] = parser.parse(str.substr(0, ind));
				}
				size_t degr = std::stoi(str.substr(ind + 2));
				resizeRes(res, degr);
				res[res.size() - 1 - degr] = parser.parse(str.substr(0, ind));
			}

			void resizeRes(std::vector<Expression<Complex>>& res, const size_t degree) const
			{
				size_t curDegre = res.size() - 1;
				if (curDegre >= degree)
					return;
				res.insert(res.begin(), degree - curDegre, Expression<Complex>());
			}
		};

	}

}
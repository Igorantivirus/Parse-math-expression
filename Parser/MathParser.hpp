#pragma once

#include "ParseUtils.hpp"

namespace expr
{
	namespace parse
	{
		template<typename Complex>
		class MathParser
		{
		public:

			Expression<Complex> parse(std::string str) const
			{
				if (!isGoodBrackets(str))
					throw ParseException("Brackets are not good", ParseException::ErrorT::brackets);
				mconverter.replaceConstants(str);

				std::vector<std::string> tokens;
				tok.tokenizer(str, tokens);

				Expression<Complex> res;
				fillExpression(tokens, res);

				return res;
			}

		private:

			Tokenizer tok;

		private:

			void strParse(std::string str, Expression<Complex>& res) const
			{
				if (str.empty())
					throw ParseException("Empty string");
				if (str[0] == '(')
				{
					if (str.size() < 3)
						throw ParseException("Empty bracket", ParseException::ErrorT::brackets);
					str.pop_back();
					str.erase(str.begin());
				}
				std::vector<std::string> tkns;
				tok.tokenizer(str, tkns);
				fillExpression(tkns, res);
			}

			void fillExpression(const std::vector<std::string>& tkns, Expression<Complex>& expr) const
			{
				if (tkns.empty())
					throw ParseException("Empty string");
				mathWorker::MathWorker<Complex> worker;
				for (size_t i = 0; i < tkns.size(); ++i)
				{
					if (tkns[i].empty())
						throw ParseException("Empty noken.");
					if (mconverter.isNum(tkns[i]))
					{
						Value<Complex> prv = worker.toComplex(tkns[i]);
						fillPostfix(tkns, i, expr, prv);
						continue;
					}
					if (tkns[i][0] == '(')
					{
						Expression<Complex> pre;
						strParse(tkns[i], pre);
						fillPostfix(tkns, i, expr, pre);
						continue;
					}
					if (mconverter.isAction(tkns[i]))
					{
						ActionT act = static_cast<ActionT>(tkns[i][0]);
						if (act == ActionT::minus)
						{
							Value<Complex> minus(Complex(-1), ActionT::multiply);
							expr.add(minus);
							continue;
						}
						else
							throw ParseException("Double arithmetic operation", ParseException::ErrorT::action);
					}
					char id = 0;
					MathBase::MathType type = mconverter.toTOT(tkns[i], id);
					if (type == MathBase::MathType::function)
						fillFunction(tkns, i, expr, id);
					else if (type == MathBase::MathType::twoFunction)
						fillTwoFunction(tkns, i, expr, id);
					else
						throw ParseException("Unknown type of token: \"" + tkns[i] + '\"', ParseException::ErrorT::word);
				}
			}

		private:

			void fillPostfix(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, MathBase& value) const
			{
				if (i + 1 < tkns.size())
				{
					if (mconverter.isAction(tkns[i + 1]))
					{
						value.setAct(static_cast<ActionT>(tkns[i + 1][0]));
						++i;
					}
					else
					{
						char id = 0;
						MathBase::MathType type = mconverter.toTOT(tkns[i + 1], id);
						if (type == MathBase::MathType::postfixFunction)
						{
							++i;
							PostfixFunction<Complex> f;
							f.setType(static_cast<PostfixFunctionT>(id));
							f.setArgument(value);
							fillPostfix(tkns, i, expr, f);
							return;
						}
						else
							value.setAct(ActionT::hiddMultiply);
					}
				}
				expr.add(value);
			}

			void fillFunction(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, const char id) const 
			{
				StanartFunction<Complex> prf;
				prf.setType(static_cast<FunctionT>(id));
				++i;
				if (i == tkns.size())
					throw ParseException("Empty function", ParseException::ErrorT::func);
				Expression<Complex> pr;
				strParse(tkns[i], pr);
				prf.setArgument(pr);
				fillPostfix(tkns, i, expr, prf);
			}

			void fillTwoFunction(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, const char id) const
			{
				TwoPramFunction<Complex> f;
				f.setType(static_cast<TwoFunctionT>(id));
				++i;
				if (i + 1 >= tkns.size())
					throw ParseException("Empty function", ParseException::ErrorT::func);
				Expression<Complex> pr1, pr2;
				strParse(tkns[i++], pr1);
				strParse(tkns[i], pr2);
				f.setArgument(pr2);
				f.setSpecilAgrument(pr1);
				fillPostfix(tkns, i, expr, f);
			}

		};

	}
}
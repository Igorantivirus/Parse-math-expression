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
				preproc(str);

				std::vector<std::string> tokens;
				tok.tokenizer(str, tokens);

				Expression<Complex> res;
				fillExpression(tokens, res);

				return res;
			}

		private:

			Tokenizer<Complex> tok;

		private:

			void preproc(std::string& str) const
			{
				Preprocessor<Complex> proc;
				proc.fullPreprocess(str);
				if (!proc.isGoodBrackets(str))
					throw ParseException("Brackets are not good", ParseException::ErrorT::brackets);
			}

		private:

			void strParse(std::string str, Expression<Complex>& res) const
			{
				if (str.empty())
					throw ParseException("Empty string");
				if (parseFuncs::isOpenBracket(str[0]))
				{
					if (str.size() < 3)
						throw ParseException("Empty bracket", ParseException::ErrorT::brackets);
					BracketT brk = mconverter.toBracket(str[0]);
					res.setBracket(brk);
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
					if (parseFuncs::isNum(tkns[i]))
					{
						Value<Complex> prv = worker.toComplex(tkns[i]);
						fillAction(tkns, i, expr, prv);
						continue;
					}
					char id = 0;
					TypeOfType type = mconverter.toTOT(tkns[i], id);
					if (type == TypeOfType::bracket)
					{
						Expression<Complex> pre;
						strParse(tkns[i], pre);
						fillAction(tkns, i, expr, pre);
					}
					else if (type == TypeOfType::action)
					{
						if (id == static_cast<char>(ActionT::minus))
						{
							Value<Complex> minus(Complex(-1), ActionT::multiply);
							expr.add(minus);
							continue;
						}
						else
							throw ParseException("Double arithmetic operation", ParseException::ErrorT::action);
					}
					else if (type == TypeOfType::func)
						fillFunction(tkns, i, expr, id);
					else if (type == TypeOfType::tFunc)
						fillTwoFunction(tkns, i, expr, id);
					else
						throw ParseException("Unknown type of token: \"" + tkns[i] + '\"', ParseException::ErrorT::word);
				}
			}

		private:

			void fillAction(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, MathBase& value) const
			{
				if (i + 1 < tkns.size())
				{
					char id = 0;
					TypeOfType type = mconverter.toTOT(tkns[i + 1], id);

					if (type == TypeOfType::action)
					{
						value.setAct(static_cast<ActionT>(id));
						++i;
					}
					else if (type == TypeOfType::pFunc)
					{
						++i;
						PostfixFunction<Complex> f;
						f.setType(static_cast<PostfixFunctionT>(id));
						f.setArgument(value);
						fillAction(tkns, i, expr, f);
						return;
					}
					else
						value.setAct(ActionT::hiddMultiply);
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
				fillAction(tkns, i, expr, prf);
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
				fillAction(tkns, i, expr, f);
			}

		};

	}
}
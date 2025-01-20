#pragma once

#include "ParseUtils.hpp"
#include "../Polinomial/Polinomial.hpp"
#include "../Polinomial/PolinomExpression.hpp"

namespace expr
{
	namespace parse
	{

		template<typename Complex>
		class PolinomParser
		{
		public:

			Polinomial<Complex> parse(std::string str) const
			{
				preproc(str);

				return subParse(str);
			}

		private:

			Tokenizer tok;

		private:

			using PolinomExpression = std::vector<std::pair<Polinomial<Complex>, ActionT>>;

		private:

			void preproc(std::string& str) const
			{
				Preprocessor proc;
				proc.constantEdding(mconverter.getConstantDictionary(), str);
				if (!proc.isGoodBrackets(str))
					throw ParseException("Brackets are not good", ParseException::ErrorT::brackets);
			}

			bool isLetter(const std::string& s) const
			{
				if (s.size() != 1)
					return false;
				const char c = s[0];
				return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
			}
			bool isSign(const std::string& s)
			{
				if (s.size() != 1)
					return false;
				const char c = s[0];
				return c == '+' || c == '-' || c == '*' || c == '/';
			}

			Polinomial<Complex> subParse(std::string str) const
			{
				if (parseFuncs::isOpenBracket(str[0]))
				{
					if (str.size() < 3)
						throw ParseException("Empty bracket", ParseException::ErrorT::brackets);
					str.pop_back();
					str.erase(str.begin());
				}

				std::vector<std::string> tokens;
				tok.tokinizerPolinom(str, tokens);

				PolinomExpression<Complex> expr;
				fillPolinom(tokens, expr);
				return expr.procession();
			}

			void fillPolinom(const std::vector<std::string>& tkns, PolinomExpression<Complex>& expr)
			{
				if (tkns.empty())
					throw ParseException("Empty string");
				mathWorker::MathWorker<Complex> worker;
				for (size_t i = 0; i < tkns.size(); ++i)
				{
					if (parseFuncs::isNum(tkns[i]))
					{
						Polinomial<Complex> pr = Monomial(worker.toComplex(tkns[i]));
						expr.add(pr, getNextAction(tkns, i));
						continue;
					}
					if (tkns[i].size() != 1)
					{
						Polinomial<Complex> pr = subParse(tkns[i]);
						expr.add(pr, getNextAction(tkns, i));
						continue;
					}
					const char c = tkns[i][0];
					if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
					{
						Polinomial<Complex> pr = Monomial(1, c);
						expr.add(pr, getNextAction(tkns, i));
						continue;
					}
					throw ParseException("Unknown Token: \"" + tkns[i] + '\"');
				}
			}

			ActionT getNextAction(const std::vector<std::string>& tkns, size_t& ind)
			{
				if (ind == tkns.size() - 1)
					return ActionT::none;
				char id = 0;
				TypeOfType type = mconverter.toTOT(tkns[i + 1], id);

				if (type == TypeOfType::action)
				{
					++ind;
					return static_cast<ActionT>(id);
				}
				return ActionT::hiddMultiply;
			}

		//	void strParse(std::string str, Expression<Complex>& res) const
		//	{
		//		if (str.empty())
		//			throw ParseException("Empty string");
		//		if (parseFuncs::isOpenBracket(str[0]))
		//		{
		//			if (str.size() < 3)
		//				throw ParseException("Empty bracket", ParseException::ErrorT::brackets);
		//			BracketT brk = mconverter.toBracket(str[0]);
		//			res.setBracket(brk);
		//			str.pop_back();
		//			str.erase(str.begin());
		//		}
		//		std::vector<std::string> tkns;
		//		tok.tokenizer(str, tkns);
		//		fillExpression(tkns, res);
		//	}

		//	void fillExpression(const std::vector<std::string>& tkns, Expression<Complex>& expr) const
		//	{
		//		if (tkns.empty())
		//			throw ParseException("Empty string");
		//		mathWorker::MathWorker<Complex> worker;
		//		for (size_t i = 0; i < tkns.size(); ++i)
		//		{
		//			if (parseFuncs::isNum(tkns[i]))
		//			{
		//				Value<Complex> prv = worker.toComplex(tkns[i]);
		//				fillAction(tkns, i, expr, prv);
		//				continue;
		//			}
		//			char id = 0;
		//			TypeOfType type = mconverter.toTOT(tkns[i], id);
		//			if (type == TypeOfType::bracket)
		//			{
		//				Expression<Complex> pre;
		//				strParse(tkns[i], pre);
		//				fillAction(tkns, i, expr, pre);
		//			}
		//			else if (type == TypeOfType::action)
		//			{
		//				if (id == static_cast<char>(ActionT::minus))
		//				{
		//					Value<Complex> minus(Complex(-1), ActionT::multiply);
		//					expr.add(minus);
		//					continue;
		//				}
		//				else
		//					throw ParseException("Double arithmetic operation", ParseException::ErrorT::action);
		//			}
		//			else if (type == TypeOfType::func)
		//				fillFunction(tkns, i, expr, id);
		//			else if (type == TypeOfType::tFunc)
		//				fillTwoFunction(tkns, i, expr, id);
		//			else
		//				throw ParseException("Unknown type of token: \"" + tkns[i] + '\"', ParseException::ErrorT::word);
		//		}
		//	}

		//private:

		//	void fillAction(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, MathBase& value) const
		//	{
		//		if (i + 1 < tkns.size())
		//		{
		//			char id = 0;
		//			TypeOfType type = mconverter.toTOT(tkns[i + 1], id);

		//			if (type == TypeOfType::action)
		//			{
		//				value.setAct(static_cast<ActionT>(id));
		//				++i;
		//			}
		//			else if (type == TypeOfType::pFunc)
		//			{
		//				++i;
		//				PostfixFunction<Complex> f;
		//				f.setType(static_cast<PostfixFunctionT>(id));
		//				f.setArgument(value);
		//				fillAction(tkns, i, expr, f);
		//				return;
		//			}
		//			else
		//				value.setAct(ActionT::hiddMultiply);
		//		}
		//		expr.add(value);
		//	}

		//	void fillFunction(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, const char id) const
		//	{
		//		StanartFunction<Complex> prf;
		//		prf.setType(static_cast<FunctionT>(id));
		//		++i;
		//		if (i == tkns.size())
		//			throw ParseException("Empty function", ParseException::ErrorT::func);
		//		Expression<Complex> pr;
		//		strParse(tkns[i], pr);
		//		prf.setArgument(pr);
		//		fillAction(tkns, i, expr, prf);
		//	}

		//	void fillTwoFunction(const std::vector<std::string>& tkns, size_t& i, Expression<Complex>& expr, const char id) const
		//	{
		//		TwoPramFunction<Complex> f;
		//		f.setType(static_cast<TwoFunctionT>(id));
		//		++i;
		//		if (i + 1 >= tkns.size())
		//			throw ParseException("Empty function", ParseException::ErrorT::func);
		//		Expression<Complex> pr1, pr2;
		//		strParse(tkns[i++], pr1);
		//		strParse(tkns[i], pr2);
		//		f.setArgument(pr2);
		//		f.setSpecilAgrument(pr1);
		//		fillAction(tkns, i, expr, f);
		//	}

		};

	}
}
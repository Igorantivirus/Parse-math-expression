#pragma once

#include "ParseUtils.hpp"
#include "../Polinomial/Polinomial.hpp"

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

				std::vector<std::string> tokens;
				tok.tokinizerPolinom(str, tokens);

				PolinomExpression expr;
				fillPolinom(tokens, expr);

				return procession(expr);
			}

		private:

			Tokenizer tok;

		private:

			using PolinomExpression = std::vector<std::pair<Polinomial<Complex>, ActionT>>;

		private:

			void preproc(std::string& str) const
			{
				Preprocessor proc;
				proc.constantEdding(MathConverter::singl().getConstantDictionary(), str);
				if (!proc.isGoodBrackets(str))
					throw ParseException("Brackets are not good", ParseException::ErrorT::brackets);
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

				PolinomExpression expr;
				fillPolinom(tokens, expr);

				return procession(expr);
			}

			void fillPolinom(const std::vector<std::string>& tkns, PolinomExpression& expr) const
			{
				if (tkns.empty())
					throw ParseException("Empty string");
				mathWorker::MathWorker<Complex> worker;
				for (size_t i = 0; i < tkns.size(); ++i)
				{
					if (parseFuncs::isNum(tkns[i]))
					{
						Polinomial<Complex> pr = Monomial(worker.toComplex(tkns[i]));
						expr.push_back({pr, getNextAction(tkns, i) });
						continue;
					}
					if (tkns[i].size() != 1)
					{
						Polinomial<Complex> pr = subParse(tkns[i]);
						expr.push_back({ pr, getNextAction(tkns, i) });
						continue;
					}
					const char c = tkns[i][0];
					if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
					{
						Polinomial<Complex> pr = Monomial<Complex>(1, c);
						expr.push_back({ pr, getNextAction(tkns, i) });
						continue;
					}
					if (c == '-')
					{
						Polinomial<Complex> pr(Monomial(Complex(-1)));
						expr.push_back({ pr, ActionT::multiply });
						continue;
					}
					throw ParseException("Unknown Token: \"" + tkns[i] + '\"');
				}
			}

			ActionT getAct(const std::string& s) const
			{
				if (s.size() != 1)
					return ActionT::none;
				char c = s[0];
				if (c == '+')
					return ActionT::plus;
				if (c == '-')
					return ActionT::minus;
				if (c == '*')
					return ActionT::multiply;
				if (c == '/')
					return ActionT::div;
				if (c == '^')
					return ActionT::pow;
			}

			ActionT getNextAction(const std::vector<std::string>& tkns, size_t& ind) const
			{
				if (ind == tkns.size() - 1)
					return ActionT::none;
				
				
				/*ActionT act = getAct(tkns[ind + 1]);
				if(act != ActionT::none)
				{
					++ind;
					return static_cast<ActionT>(id);
				}
				return ActionT::hiddMultiply;*/
				
				char id = 0;
				TypeOfType type = MathConverter::singl().toTOT(tkns[ind + 1], id);

				if (type == TypeOfType::action)
				{
					++ind;
					return static_cast<ActionT>(id);
				}
				return ActionT::hiddMultiply;
			}

		private:

			bool isZ(const Complex a) const
			{
				mathWorker::MathWorker<Complex> wrkr;

				return std::abs(a.imag()) < 1e-10 && std::abs(a.real() - (long long)(a.real())) < 1e-10;
			}

			void procPow(PolinomExpression& expr) const
			{
				for (size_t i = 0; i < expr.size() - 1; ++i)
				{
					if (expr[i].second != ActionT::pow)
						continue;
					Polinomial<Complex> degr = expr[i + 1].first;
					if (!(degr.size() == 1 && degr[0].getCoefs().size() <= 1))
						throw ParseException("Error of pow.");
					Complex pow = degr[0].getNum();
					if(!isZ(pow))
						throw ParseException("Error of pow.");

					unsigned int realPow = static_cast<unsigned int>(pow.real());

					expr[i].first = Polinomial<Complex>::pow(expr[i].first, realPow);
					expr[i].second = expr[i + 1].second;
					expr.erase(expr.begin() + i + 1);
					--i;
				}
			}

			void procMul(PolinomExpression& expr) const
			{
				for (size_t i = 0; i < expr.size() - 1; ++i)
				{
					if (expr[i].second == ActionT::multiply || expr[i].second == ActionT::hiddMultiply)
					{
						Polinomial<Complex> b = expr[i + 1].first;
						expr[i].first *=  b;
						expr[i].second = expr[i + 1].second;
						expr.erase(expr.begin() + i + 1);
						--i;
					}
					else if (expr[i].second == ActionT::div)
					{
						Polinomial<Complex> b = expr[i + 1].first;
						if (!(expr[i].first /= b))
							throw ParseException("Error div.");
						expr[i].second = expr[i + 1].second;
						expr.erase(expr.begin() + i + 1);
						--i;
					}
				}
			}

			Polinomial<Complex> procession(PolinomExpression& expr) const
			{
				if (expr.empty())
					throw ParseException("Empty expr");
				procPow(expr);
				procMul(expr);
				Polinomial<Complex> res;
				for (size_t i = 0; i < expr.size() - 1; ++i)
				{
					if (expr[i].second != ActionT::plus && expr[i].second != ActionT::minus)
						throw ParseException("Unknown operation.");
					Polinomial<Complex> b = expr[i + 1].first;
					if (expr[i].second == ActionT::plus)
						expr[i].first += b;
					else
						expr[i].first -= b;
					expr[i].second = expr[i + 1].second;
					expr.erase(expr.begin() + i + 1);
					--i;
				}
				return expr[0].first;
			}


		};

	}
}
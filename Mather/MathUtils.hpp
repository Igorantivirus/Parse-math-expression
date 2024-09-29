#pragma once

#include<vector>
#include<string>
#include<memory>
#include<map>

#include"../MathWorker/MathWorker.hpp"
#include"../FileHeaders/FileReader.hpp"

#define BD_TD_FILE "bd.td"

namespace expr
{

	enum class ActionT : char
	{
		none = 0,
		plus,
		minus,
		multiply,
		div,
		wholeDiv,
		mod,
		pow,
		hiddMultiply
	};

	enum class FunctionT : char
	{
		none = 0,

		sqrt,
		ln,
		lg,
		exp,
		abs,

		sin,
		cos,
		tg,
		ctg,

		arcsin,
		arccos,
		arctg,
		arcctg,
	};

	enum class TwoFunctionT : char
	{
		none = 0,
		log,
		root
	};

	enum class PostfixFunctionT : char
{
		none = 0,
		degree,
		radian,
		factorial
	};

	enum class BracketT : char
{
		standart = 0,
		whole,
		fract,
		modul
	};

	enum class TypeOfType : char
	{
		none = 0,
		action = 1,
		func = 2,
		tFunc = 3,
		pFunc = 4,
		bracket = 5
	};

	namespace solver
	{

		template<typename Complex>
		Complex solve(const Complex& arg1, const Complex& arg2, const ActionT type) {
			mathWorker::MathWorker<Complex> worker;
			switch (type)
			{
			case ActionT::plus: return arg1 + arg2;
			case ActionT::minus: return arg1 - arg2;
			case ActionT::multiply: return arg1 * arg2;
			case ActionT::hiddMultiply: return arg1 * arg2;
			case ActionT::div: return arg1 / arg2;
			case ActionT::wholeDiv: return worker.floor(arg1 / arg2);
			case ActionT::mod: return worker.fmod(arg1, arg2);
			case ActionT::pow: return worker.pow(arg1, arg2);
			default: return worker.getNan();
			}
			return worker.getNan();
		}

		template<typename Complex>
		Complex solve(const Complex& v, const FunctionT type) {
			mathWorker::MathWorker<Complex> worker;
			switch (type)
			{
			case FunctionT::exp: return worker.exp(v);
			case FunctionT::sqrt: return worker.sqrt(v);
			case FunctionT::abs: return worker.abs(v);
			case FunctionT::ln: return worker.ln(v);
			case FunctionT::lg: return worker.lg(v);
			case FunctionT::sin: return worker.sin(v);
			case FunctionT::cos: return worker.cos(v);
			case FunctionT::tg: return worker.tg(v);
			case FunctionT::ctg: return worker.ctg(v);
			case FunctionT::arcsin: return worker.arcsin(v);
			case FunctionT::arccos: return worker.arccos(v);
			case FunctionT::arctg: return worker.arctg(v);
			case FunctionT::arcctg: return worker.arcctg(v);
			default: return worker.getNan();
			}
			return worker.getNan();
		}

		template<typename Complex>
		Complex solve(const Complex& arg, const Complex& sarg, const TwoFunctionT type)
		{
			mathWorker::MathWorker<Complex> worker;
			switch (type)
			{
			case TwoFunctionT::log: return worker.log(sarg, arg);
			case TwoFunctionT::root: return worker.nrt(arg, sarg);
			default: return worker.getNan();
			}
			return worker.getNan();
		}

		template<typename Complex>
		Complex solve(const Complex& v, const PostfixFunctionT type) {
			mathWorker::MathWorker<Complex> worker;
			switch (type)
			{
			case PostfixFunctionT::degree: return worker.toDegree(v);
			case PostfixFunctionT::radian: return worker.toRadian(v);
			case PostfixFunctionT::factorial: return worker.factorial(v);
			default: return worker.getNan();
			}
			return worker.getNan();
		}

		template<typename Complex>
		Complex solve(const Complex& v, const BracketT type) {
			mathWorker::MathWorker<Complex> worker;
			switch (type)
			{
			case BracketT::standart: return v;
			case BracketT::whole: return worker.floor(v);
			case BracketT::fract: return v - worker.floor(v);
			case BracketT::modul: return worker.abs(v);
			default: return worker.getNan();
			}
			return worker.getNan();
		}

	}

	class MathConverter {
	public:

		using Сouple = struct
		{
			char id;
			TypeOfType type;
		};

	public:
		MathConverter(const std::string& path)
		{
			readMap(path);
		}

		void reopen(const std::string& str)
		{
			readMap(str);
		}

		const std::string toStr(const ActionT v) const
		{
			for (auto const& [key, value] : _map)
				if (value.type == TypeOfType::action && value.id == static_cast<char>(v))
					return key;
			return "";
		}
		const std::string toStr(const FunctionT v) const
		{
			for (auto const& [key, value] : _map)
				if (value.type == TypeOfType::func && value.id == static_cast<char>(v))
					return key;
			return "";
		}
		const std::string toStr(const TwoFunctionT v) const
		{
			for (auto const& [key, value] : _map)
				if (value.type == TypeOfType::tFunc && value.id == static_cast<char>(v))
					return key;
			return "";
		}
		const std::string toStr(const PostfixFunctionT v) const
		{
			for (auto const& [key, value] : _map)
				if (value.type == TypeOfType::pFunc && value.id == static_cast<char>(v))
					return key;
			return "";
		}

		std::string toStrOpen(const BracketT v) const
		{
			if (v == BracketT::standart) return "(";
			if (v == BracketT::whole) return "[";
			if (v == BracketT::fract) return "{";
			if (v == BracketT::modul) return "<";
			return "";
		}
		std::string toStrClose(const BracketT v) const
		{
			if (v == BracketT::standart) return ")";
			if (v == BracketT::whole) return "]";
			if (v == BracketT::fract) return "}";
			if (v == BracketT::modul) return ">";
			return "";
		}
		BracketT toBracket(const char c) const
		{
			if (c == '[' || c == ']') return BracketT::whole;
			if (c == '{' || c == '}') return BracketT::fract;
			if (c == '<' || c == '>') return BracketT::modul;
			return BracketT::standart;
		}
		char oppositeBracket(const char c) const
		{
			if (c == '(')return ')';
			if (c == '[')return ']';
			if (c == '{')return '}';
			if (c == '<')return '>';
			if (c == ')')return '(';
			if (c == ']')return '[';
			if (c == '}')return '{';
			if (c == '>')return '<';
			return c;
		}

		const TypeOfType toTOT(const std::string& s, char& id) const
		{
			if (s[0] == '(') return (id = static_cast<char>(BracketT::standart)), TypeOfType::bracket;
			if (s[0] == '[') return (id = static_cast<char>(BracketT::whole)), TypeOfType::bracket;
			if (s[0] == '{') return (id = static_cast<char>(BracketT::fract)), TypeOfType::bracket;
			if (s[0] == '<') return (id = static_cast<char>(BracketT::modul)), TypeOfType::bracket;
			if (auto iter = _map.find(s); iter != _map.end())
			{
				id = iter->second.id;
				return iter->second.type;
			}
			return TypeOfType::none;
		}

		const std::map<std::string, std::string>& getConstantDictionary() const
		{
			return _constants;
		}

	private:

		std::map<std::string, Сouple> _map;
		std::map<std::string, std::string> _constants;

	private:

		void readMap(const std::string& path)
		{
			_map.clear();
			_constants.clear();
			TDReader file(path);
			if (!file.isOpen())
			{
				//std::cerr << "Not found \"" << path << '\"' << '\n';
				return;
			}
			std::vector<std::string> prArr;
			while ((prArr = file.readLine()).size() == 3)
			{
				char idTOT = std::stoi(prArr[1]);
				if (idTOT == 5)
					_constants.insert(std::make_pair(prArr[0], prArr[2]));
				else
				{
					char id = std::stoi(prArr[2]);
					_map.insert(std::make_pair(prArr[0], Сouple{ id, static_cast<TypeOfType>(idTOT) }));
				}
			}
			file.close();
		}

	};

	MathConverter mconverter(BD_TD_FILE);

	void replaceAll(std::string& str, const std::string& oldS, const std::string& newS) 
	{
		const size_t s1 = oldS.size();
		const size_t s2 = newS.size();
		size_t ind;
		size_t last = 0;
		while ((ind = str.find(oldS, last)) != std::string::npos)
		{
			str.replace(ind, s1, newS);
			last = ind + s2;
		}
	}

}
#pragma once

#include"MathUtils.hpp"

namespace expr
{

	class MathConverter {
	public:

		using Ñouple = struct
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

		std::map<std::string, Ñouple> _map;
		std::map<std::string, std::string> _constants;

	private:

		void readMap(const std::string& path)
		{
			_map.clear();
			_constants.clear();
			TDReader file(path);
			if (!file.isOpen())
			{
				std::cerr << "Not found bd.td file on path: \"" << path << '\"' << '\n';
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
					_map.insert(std::make_pair(prArr[0], Ñouple{ id, static_cast<TypeOfType>(idTOT) }));
				}
			}
			file.close();
		}

	};

	MathConverter mconverter(BD_TD_FILE);
}
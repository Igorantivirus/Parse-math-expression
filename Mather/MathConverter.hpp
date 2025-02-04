#pragma once

#include"MathUtils.hpp"

namespace expr
{

	class MathConverter
	{
	public:
		MathConverter(const std::string& path)
		{
			readMap(path);
		}

		void reopen(const std::string& str)
		{
			readMap(str);
		}

		bool isAction(const std::string& s) const
		{
			
			return s.size() == 1 && (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/' || s[0] == '%' || s[0] == '^');
		}

		bool isNum(const char c)
		{
			return (c >= '0' && c <= '9');
		}
		bool isNum(const std::string& s)
		{
			return isNum(s[0]) || (s.size() == 1 && s[0] == 'i') || (s.size() >= 2 && (s[0] == '-' || s[0] == '.') && isNum(s[1])) || s == "nan" || s == "-nan" || s == "inf" || s == "-inf";
		}

		const std::string toStr(const ActionT v) const
		{
			return std::string(1, static_cast<char>(v));
		}
		const std::string toStr(const FunctionT v) const
		{
			return toStrTempl(MathBase::MathType::function, static_cast<char>(v));
		}
		const std::string toStr(const TwoFunctionT v) const
		{
			return toStrTempl(MathBase::MathType::twoFunction, static_cast<char>(v));
		}
		const std::string toStr(const PostfixFunctionT v) const
		{
			return toStrTempl(MathBase::MathType::postfixFunction, static_cast<char>(v));
		}

		char oppositeBracket(const char c) const
		{
			if (c == '(') return ')';
			if (c == ')') return '(';
			return c;
		}

		const MathBase::MathType toTOT(const std::string& s, char& id) const
		{
			if (auto iter = _map.find(s); iter != _map.end())
			{
				id = iter->second.first;
				return iter->second.second;
			}
			return MathBase::MathType::none;
		}

		const std::map<std::string, std::string>& getConstantDictionary() const
		{
			return _constants;
		}
		const std::map<std::string, std::pair<char, MathBase::MathType>>& getMap() const
		{
			return _map;
		}

		void replaceConstants(std::string& str) const
		{
			for (const auto& [key, value] : _constants)
				replaceAll(str, key, value);
		}

	private:

		std::map<std::string, std::pair<char, MathBase::MathType>> _map;
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
					_map.insert(std::make_pair(prArr[0], std::make_pair( id, static_cast<MathBase::MathType>(idTOT) )));
				}
			}
			file.close();
		}

		std::string toStrTempl(const MathBase::MathType tot, const char v) const
		{
			for (auto const& [key, value] : _map)
				if (value.second == tot && value.first == v)
					return key;
			return "";
		}

		void replaceAll(std::string& str, const std::string& oldS, const std::string& newS) const
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

	};

	MathConverter mconverter(BD_TD_FILE);
}
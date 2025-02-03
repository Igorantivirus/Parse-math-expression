//#pragma once
//
//#include "MathUtils.hpp"
//#include "BaseClasses.hpp"
//
//namespace expr
//{
//	class MathConverter
//	{
//	public:
//		MathConverter(const std::string& path)
//		{
//			readMap(path);
//		}
//
//		void reopen(const std::string& str)
//		{
//			readMap(str);
//		}
//
//		const std::string toStr(const ActionT v) const
//		{
//			return std::string(1, static_cast<char>(v));
//		}
//		const std::string toStr(const FunctionT v) const
//		{
//			return toStrTampl(TypeOfType::func, static_cast<char>(v));
//		}
//		const std::string toStr(const TwoFunctionT v) const
//		{
//			return toStrTampl(TypeOfType::tFunc, static_cast<char>(v));
//
//		}
//		const std::string toStr(const PostfixFunctionT v) const
//		{
//			return toStrTampl(TypeOfType::pFunc, static_cast<char>(v));
//			
//		}
//
//		char oppositeBracket(const char c) const
//		{
//			if (c == '(') return ')';
//			if (c == ')') return '(';
//			return c;
//		}
//
//		const TypeOfType toTOT(const std::string& s, char& id) const
//		{
//			if (auto iter = _map.find(s); iter != _map.end())
//			{
//				id = iter->second.first;
//				return iter->second.second;
//			}
//			return TypeOfType::none;
//		}
//
//		const std::map<std::string, std::string>& getConstantDictionary() const
//		{
//			return _constants;
//		}
//
//	private:
//
//		std::map<std::string, std::pair<char, MathBase::MathType>> _map;
//		std::map<std::string, std::string> _constants;
//
//	private:
//
//		void readMap(const std::string& path)
//		{
//			_map.clear();
//			_constants.clear();
//			TDReader file(path);
//			if (!file.isOpen())
//			{
//				std::cerr << "Not found bd.td file on path: \"" << path << '\"' << '\n';
//				return;
//			}
//
//			std::vector<std::string> prArr;
//			while ((prArr = file.readLine()).size() == 3)
//			{
//				char idTOT = std::stoi(prArr[1]);
//				char id = std::stoi(prArr[2]);
//				_map.insert(std::make_pair(prArr[0], std::make_pair(id, static_cast<TypeOfType>(idTOT))));
//			}
//			file.close();
//		}
//
//		std::string toStrTampl(const TypeOfType type, const char v) const
//		{
//			for (auto const& [key, value] : _map)
//				if (value.second == type && value.first == v)
//					return key;
//			return "";
//		}
//
//	};
//
//	MathConverter mconverter(BD_TD_FILE);
//}
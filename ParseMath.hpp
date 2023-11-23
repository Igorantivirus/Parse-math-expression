#pragma once

#include<string>
#include<cmath>
#include<vector>
#include<iostream>

struct Math
{
public:
	static void PrinaEvalInfo()
	{
		std::cout << "The method accepts a string, which is a mathematical expression.		" << std::endl;
		std::cout << "Returns the result of this expression.								" << std::endl;
		std::cout << "The method works with such mathematical operators and functions:		" << std::endl;
		std::cout << "!																		" << std::endl;
		std::cout << "sin cos tg ctg arcsin arccos arctg arcctg								" << std::endl;
		std::cout << "^   sqr log lg ln														" << std::endl;
		std::cout << "*   /   //  %															" << std::endl;
		std::cout << "+   -																	" << std::endl;
		std::cout << "Perceives the number \"pi\" and \"e\"									" << std::endl;
	}
	static double Eval(std::string txt)
	{
		double res;
		try
		{
			EditStr(txt);
			Skobki(txt);
			EvalMini(txt);
			res = std::stod(txt);
		}
		catch (...)
		{
			if (txt.find("-nan") != std::string::npos)
				return std::stod("-nan");
			return std::stod("nan");
		}
		return res;
	}
private:
	static void Skobki(std::string& txt)
	{
		size_t pos1 = 0, pos2 = 0, pos;
		std::string res, last = txt;
		char skobka = '(';
		while ((pos2 = std::min(std::min(txt.find(')'), txt.find('>')), std::min(txt.find(']'), txt.find('}')))) != std::string::npos)
		{
			skobka = EditChar(txt[pos2]);
			for (size_t j = pos2; j != std::string::npos; j--)
			{
				if (txt[j] == skobka)
				{
					pos1 = j;
					break;
				}
			}
			if (txt[pos1] != skobka)
				return;
			res = txt.substr(pos1 + 1, pos2 - pos1 - 1);
			EvalMini(res);
			if (skobka == '[')
				res = std::to_string(floor(stod(res)));
			else if (skobka == '{')
			{
				double pr = stod(res);
				res = std::to_string(pr - floor(pr));
			}
			else if (skobka == '<')
				res = std::to_string(abs(stod(res)));
			if (res.find('.') != std::string::npos)
				while (res[res.size() - 1] == '0')
					res.erase(res.size() - 1);
			if (res[res.size() - 1] == '.')
				res.erase(res.size() - 1);
			txt.replace(pos1, pos2 - pos1 + 1, res);
			if (last == txt)
				return;
			else
				last = txt;
		}
	}
	static void EvalFoos(std::string& txt)
	{
		static std::vector<std::string> arr = { "ln","lg","sin","cos","tg","ctg","arcsin","arccos","arctg","arcctg" };
		std::string pr = "", prS, sign, last = txt;
		size_t pos;
		size_t i = 0;
		while ((pos = MinOfFind(arr, txt, sign)) != std::string::npos)
		{
			if (txt[pos + sign.size()] == '-')
			{
				pr += '-';
				i++;
			}
			FillNextStr(txt, i += pos + sign.size(), pr);
			prS = EvalNano("", sign, pr);
			EditStrDouble(prS);
			txt.replace(pos, i - pos + 1, prS);
			pr = "";
			i = 0;
			if (last == txt)
				return;
			else
				last = txt;
		}
	}
	static void EvalMini(std::string& txt)
	{
		std::vector<std::string> signs = { "!" };
		EvalSign(txt, signs);
		EvalFoos(txt);
		signs = { "^", "sqr", "log" };
		EvalSign(txt, signs);
		signs = { "//","*", "/", "%" };
		EvalSign(txt, signs);
		signs = { "+", "-" };
		EvalSign(txt, signs);
	}
	static void EvalSign(std::string& txt, const std::vector<std::string>& arr)
	{
		std::string pr1 = "", pr2 = "", prS, sign, last = txt;
		size_t pos;
		size_t i = 0, j = 0;
		while ((pos = MinOfFind(arr, txt, sign)) != std::string::npos)
		{
			if (pos == 0)
			{
				pos = MinOfFind(arr, txt, sign, 1);
				if (pos == std::string::npos)
					return;
			}
			FillNextStr(txt, i = pos + sign.size(), pr2);
			FillLastStr(txt, j = pos - 1, pr1);
			prS = EvalNano(pr1, sign, pr2);
			EditStrDouble(prS);
			txt.replace(j, i - j + 1, prS);
			pr1 = pr2 = "";
			i = j = 0;
			if (last == txt)
				return;
			else
				last = txt;
		}
	}
	static std::string EvalNano(const std::string& val1, const std::string& sign, const std::string& val2)
	{
		if (sign == "+")
			return std::to_string(stod(val1) + stod(val2));
		else if (sign == "-")
			return std::to_string(stod(val1) - stod(val2));
		else if (sign == "*")
			return std::to_string(stod(val1) * stod(val2));
		else if (sign == "/")
			return std::to_string(stod(val1) / stod(val2));
		else if (sign == "%")
			return std::to_string(fmod(stod(val1), stod(val2)));
		else if (sign == "//")
			return std::to_string(floor(stod(val1) / stod(val2)));
		else if (sign == "^")
			return std::to_string(pow(stod(val1), stod(val2)));
		else if (sign == "!")
			return std::to_string(MyFact(stod(val1) + 1.));
		else if (sign == "sqr")
			return std::to_string(MySQR(stod(val1), stod(val2)));
		else if (sign == "log")
			return std::to_string(log2(stod(val2)) / log2(stod(val1)));
		else if (sign == "ln")
			return std::to_string(log(stod(val2)));
		else if (sign == "lg")
			return std::to_string(log10(stod(val2)));
		else if (sign == "sin")
			return std::to_string(sin(stod(val2)));
		else if (sign == "cos")
			return std::to_string(cos(stod(val2)));
		else if (sign == "tg")
			return std::to_string(tan(stod(val2)));
		else if (sign == "ctg")
			return std::to_string(1. / tan(stod(val2)));
		else if (sign == "arcsin")
			return std::to_string(asin(stod(val2)));
		else if (sign == "arccos")
			return std::to_string(acos(stod(val2)));
		else if (sign == "arctg")
			return std::to_string(atan(stod(val2)));
		else if (sign == "arcctg")
			return std::to_string((3.1415926 / 2) - atan(stod(val2)));
	}
	static void replaceAll(std::string& str, const std::string& oldS, const std::string& newS)
	{
		for (size_t pos = 0; (pos = str.find(oldS)) != std::string::npos; pos += newS.size())
		{
			str.replace(pos, oldS.size(), newS);
		}
	}
	static void EditStr(std::string& str)
	{
		for (size_t pos = 0; (pos = str.find(' '), pos) != std::string::npos;)
			str.erase(pos, 1);
		ModulEdit(str);
		replaceAll(str, "pi", "3.1415926");
		replaceAll(str, "e", "2.718281828");
		replaceAll(str, ",", ".");
	}
	static void ModulEdit(std::string& str)
	{
		if (str[0] == '|')
			str[0] = '<';
		size_t pos;
		while ((pos = str.find('|')) != std::string::npos)
		{
			if (!(str[pos - 1] >= '0' && str[pos - 1] <= '9') && str[pos - 1] != ')' && str[pos - 1] != ']' && str[pos - 1] != '}' && str[pos - 1] != '|')
				str[pos] = '<';
			else
				str[pos] = '>';
		}
	}
	static size_t MinOfFind(const std::vector<std::string>& arr, const std::string& txt, std::string& sign, int pos = 0)
	{
		if (arr.empty())
			return std::string::npos;
		if (arr.size() == 1)
		{
			sign = arr[0];
			return txt.find(arr[0], pos);
		}
		size_t res = txt.find(arr[0], pos);
		sign = arr[0];
		size_t pr;
		for (size_t i = 1; i < arr.size(); i++)
		{
			pr = std::min(res, txt.find(arr[i], pos));
			if (pr != res)
			{
				res = pr;
				sign = arr[i];
			}
		}
		return res;
	}
	static char EditChar(char c)
	{
		if (c == ')')
			return '(';
		else if (c == '>')
			return '<';
		else if (c == ']')
			return '[';
		else if (c == '}')
			return '{';
		else
			return c;
	}
	static double MySQR(double stepen, double value)
	{
		double zn = 1;
		if (value < 0 && fmod(stepen, 2) == 1)
			value *= (zn = -1);
		return pow(value, 1 / stepen) * zn;
	}
	static double MyFact(double val)
	{
		if (val < 0)
			return sqrt(-1);
		return tgamma(val);
	}
	static void FillNextStr(const std::string& txt, size_t& pos, std::string& res)
	{
		if (txt[pos] == '-')
			res += txt[pos++];
		if (txt[pos] == 'i')
			if (txt[pos + 1] == 'n' && txt[pos + 2] == 'f')
			{
				res += "inf";
				pos += 2;
				return;
			}
		while (pos < txt.size() && (txt[pos] >= '0' && txt[pos] <= '9' || txt[pos] == '.'))
			res += txt[pos++];
		pos--;
	}
	static void FillLastStr(const std::string& txt, size_t& pos, std::string& res)
	{
		if (txt[pos] == 'f')
		{

			if (txt[pos - 1] == 'n' && txt[pos - 2] == 'i')
			{
				res = "inf" + res;
				pos -= 3;
			}
		}
		else {
			while (pos != std::string::npos && (txt[pos] >= '0' && txt[pos] <= '9' || txt[pos] == '.'))
				res = txt[pos--] + res;
		}
		if (pos == std::string::npos)
			pos++;
		else if (txt[pos] == '-' && !(txt[pos - 1] >= '0' && txt[pos - 1] <= '9'))
			res = '-' + res;
		else
			pos++;
	}
	static void EditStrDouble(std::string& txt)
	{
		if (txt.find('.') == std::string::npos)
			return;
		while (txt[txt.size() - 1] == '0')
			txt.erase(txt.size() - 1);
		if (txt[txt.size() - 1] == '.')
			txt.erase(txt.size() - 1);
	}
};
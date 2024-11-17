#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<vector>

#include"../Bd_td.hpp"

class TDReader
{
public:
	TDReader(const std::string& path) : _file(bd_td) {}

	std::vector<std::string> readLine()
	{
		std::string str;
		readString(str);
		deleteCommentry(str);

		std::vector<std::string> res;
		fillArray(res, str);
		return res;
	}

	bool isOpen() const
	{
		return true;
	}
	void close()
	{
		_file.clear();
	}
	void open(const std::string& path)
	{
		//_file.read();
	}

private:

	std::istringstream _file;

private:

	void readString(std::string& str) {
		while (!_file.eof() && (str.empty() || str[0] == ';'))
			std::getline(_file, str);
	}

	void deleteCommentry(std::string& str) {
		if (size_t ind = str.find(";"); ind != std::string::npos)
			str.erase(ind);
	}

	void fillArray(std::vector<std::string>& arr, const std::string& str) {
		std::string tmp;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (str[i] == ' ' || str[i] == '\t')
			{
				arr.push_back(tmp);
				tmp.clear();
			}
			else
			{
				tmp += str[i];
			}
		}
		if (!tmp.empty())
			arr.push_back(tmp);
		for (size_t i = 0; i < arr.size(); ++i)
			if (arr[i].empty())
			{
				arr.erase(arr.begin() + i);
				--i;
			}
	}

};
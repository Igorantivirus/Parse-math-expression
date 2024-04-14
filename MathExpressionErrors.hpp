#pragma once

#include<string>
#include<exception>

namespace expr
{
	class ParseException : public std::exception
	{
	public:
		enum class ErrorType : char
		{
			none = 0,
			brackets,
			word,
			func
		};
	public:
		ParseException(const std::string& str, ErrorType t = ErrorType::none) : msg(str), type(t) {}

		char const* what() const override
		{
			return msg.c_str();
		}
		const std::string& swhat() const
		{
			return msg;
		}

	private:
		std::string msg;
		ErrorType type;

	};
}
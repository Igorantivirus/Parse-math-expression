#pragma once

#include<string>
#include<exception>

namespace expr
{
	namespace parse
	{

	class ParseException : public std::exception
	{
	public:
		enum class ErrorT : char
		{
			none = 0,
			brackets,
			word,
			func,
			overflow,
			action
		};
	public:
		ParseException(const std::string& str, ErrorT t = ErrorT::none) : msg(str), type(t) {}

		char const* what() const noexcept override
		{
			return msg.c_str();
		}
		const std::string& swhat() const
		{
			return msg;
		}

		ErrorT getType() const
		{
			return type;
		}

	private:
		std::string msg;
		ErrorT type;

	};

	}
}

#include "convert_functions.h"
namespace my_convert_functions
{
	string get_string_of(string value)
	{
		return value;
	}
	string get_string_of(int64_t value)
	{
		string s = lexical_cast<string>(value);
		return s;
	}
	string get_string_of(double value)
	{
		string s = lexical_cast<string>(value);
		return s;
	}
	string get_string_of(float value)
	{
		string s = lexical_cast<string>(value);
		return s;
	}
	string get_string_of(int value)
	{
		string s = lexical_cast<string>(value);
		return s;
	}
}
#ifndef CONVERT_FUNCTIONS_H
#define CONVERT_FUNCTIONS_H
#include <string>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;
namespace my_convert_functions
{
extern string get_string_of(string value);
extern string get_string_of(int64_t value);
extern string get_string_of(double value);
extern string get_string_of(float value);
extern string get_string_of(int value);
}
#endif
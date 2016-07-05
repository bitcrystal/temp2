#ifndef BASE_64_H
#define BASE_64_H
#include <string>

extern std::string base64_encode(unsigned char const* , unsigned int len);
extern std::string base64_decode(std::string const& s);
#endif
#ifndef BASE_SECURITY_H
#define BASE_SECURITY_H
#include "base64.h"
#include "random.h"
using namespace myrand;
extern void get_encoded_table_my(char * encoded_table, unsigned int in_len);
extern std::string encode_security(char const* bytes_to_encode, unsigned int in_len);
extern std::string decode_security(std::string & encoded_string);
#endif

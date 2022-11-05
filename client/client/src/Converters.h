#pragma once
#include <string>
#include <base64.h>


class Base_64_service
{
public:
	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);
};

std::string bytes_to_hex(const uint8_t* data, int len);
std::vector<char> hex_to_bytes(const std::string& hex);

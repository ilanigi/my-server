#pragma once
#include <string>
#include <base64.h>


class Base_64_service
{
public:
	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);
};

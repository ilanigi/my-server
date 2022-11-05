#include "Converters.h"

#include <iomanip>
#include <sstream>
#include <string>

std::string bytes_to_hex(const uint8_t* data, int len)
{
	std::stringstream stream;
	stream << std::hex;

	for (int i(0); i < len; ++i)
		stream << std::setw(2) << std::setfill('0') << (int)data[i];

	return stream.str();
}

std::string Base_64_service::encode(const std::string& str)
{
	std::string encoded;
	CryptoPP::StringSource ss(str, true,
		new CryptoPP::Base64Encoder(
			new CryptoPP::StringSink(encoded)
		) 
	); 

	return encoded;
}

std::string Base_64_service::decode(const std::string& str)
{
	std::string decoded;
	CryptoPP::StringSource ss(str, true,
		new CryptoPP::Base64Decoder(
			new CryptoPP::StringSink(decoded)
		) 
	); 

	return decoded;
}


std::vector<char> hex_to_bytes(const std::string& hex) {
	std::vector<char> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		char byte = (char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}

	return bytes;
}

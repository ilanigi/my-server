#include "Secret_service.h"
#include "File_service.h"
#include "Converters.h"
#include <boost/crc.hpp >

Secret_service::Secret_service() {
	std::string base_64_private_key;

	if (File_service::file_exist(USER_FILE)) {
		base_64_private_key = File_service::get_private_key();
		if (base_64_private_key.length() > 0) {
			std::cout << "private key found in me.info file." << std::endl;
			decode_key(base_64_private_key);
		}
		else {
			// create and add to me.info
			std::cout << "crating private key" << std::endl;
			private_key.Initialize(rng, KEY_SIZE);
			add_key_to_file();
		}
	}
	else {
		std::cout << "User credentials  are not exist, please register before sending key." << std::endl;
	}

}
Secret_service::~Secret_service() {};

void Secret_service::add_key_to_file() {
	std::string base_64_key = Base_64_service::encode(get_private_key());
	File_service::add_line_to_file(USER_FILE, base_64_key);
}
void Secret_service::decode_key(std::string base_64_key) {
	std::string key = Base_64_service::decode(base_64_key);
	CryptoPP::StringSource ss(key, true);
	private_key.Load(ss);
}

char* Secret_service::get_public_key(char* buffer, unsigned int size) const
{
	CryptoPP::RSAFunction publicKey(private_key);
	CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(buffer), size);
	publicKey.Save(as);
	return buffer;
}


std::string Secret_service::get_private_key() const {
	std::string key;
	CryptoPP::StringSink ss(key);
	private_key.Save(ss);
	return key;
}

std::string Secret_service::decrypt(const char* cipher, unsigned int length) {
	std::string decrypted;
	CryptoPP::RSAES_OAEP_SHA_Decryptor d(private_key);
	CryptoPP::StringSource ss_cipher(reinterpret_cast<const CryptoPP::byte*>(cipher), length, true, new CryptoPP::PK_DecryptorFilter(rng, d, new CryptoPP::StringSink(decrypted)));
	return decrypted;
}

uint32_t Secret_service::check_sum(const char * file[], size_t size) {
	boost::crc_32_type result;
	result.process_bytes(file, size);
	return result.checksum();
}


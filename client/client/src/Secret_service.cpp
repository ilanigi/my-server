#include "Secret_service.h"
#include "File_service.h"


Secret_service::Secret_service() {
	std::string string_private_key;
	if (File_service::file_exist(USER_FILE)) {
		string_private_key = get_private_key();
		if (string_private_key.length() > 0) {
			std::cout << "private key found in  me.info file." << std::endl;
			// private key exist - modify from 64base to object.
		}
		else {
			// create and add to me.info
			std::cout << "crating private key" << std::endl;
			private_key.Initialize(rng, KEY_SIZE);

		}
		return;
	}
	else {
		std::cout << "User credentials  are not exist, please register before sending key." << std::endl;
	}

}
Secret_service::~Secret_service() {};

std::string Secret_service::get_public_key() {
	CryptoPP::RSAFunction publicKey(private_key);
	std::string key;
	CryptoPP::StringSink ss(key);
	publicKey.Save(ss);
	return key;
}

std::string Secret_service::get_private_key() {
	std::string key;
	CryptoPP::StringSink ss(key);
	private_key.Save(ss);
	return key;
}


//rsaPrivate.GenerateRandomWithKeySize(prng, 1024);


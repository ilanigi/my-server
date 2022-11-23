#pragma once
#include <rsa.h>
#include <osrng.h> 

class Secret_service {
public: 
	static const unsigned int PUBLIC_KEY_SIZE_NET = 160;
	static const unsigned int AES_KEY_SIZE = 16;
	static uint32_t check_sum(std::string );

	Secret_service();
	~Secret_service();

	void init();
	std::string get_public_key() const;
	void decrypt_key(const char* , unsigned int , unsigned char* , unsigned int );
	std::string encrypt(const char* , unsigned int );
	void set_AES_key(unsigned char* );

private:
	bool isInit = false;
	static const int BITS = 1024;
	static const int BUFFER_SIZE = 1024;

	const std::string INIT_MESSAGE = "User credentials are not exist, please register before sending key.";
	
	unsigned char AES_key[AES_KEY_SIZE] = {0};
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey private_key;

	void init_from_file(std::string);
	void add_key_to_file();
	std::string get_private_key() const;
};


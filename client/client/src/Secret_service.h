#pragma once
#include <rsa.h>
#include <osrng.h> 

class Secret_service {
public: 
	static const unsigned int PUBLIC_KEY_SIZE_NET = 160;
	static const unsigned int AES_KEY_SIZE = 16;
	Secret_service();
	~Secret_service();
	void init();
	  void get_public_key(char* , unsigned int ) const;
	  void decrypt_key(const char* , unsigned int , unsigned char* , unsigned int );
	  static uint32_t check_sum(const char* [], size_t );
	  std::string encrypt(const char* plain, unsigned int length);
	  void set_AES_key(unsigned char* buffer);
private:
	bool isInit = false;
	const std::string INIT_MESSAGE = "User credentials are not exist, please register before sending key.";
	
	static const unsigned int BITS = 1024;
	unsigned char AES_key[AES_KEY_SIZE] = {0};
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey private_key;
	void init_from_file(std::string);
	std::string get_private_key() const;
	void add_key_to_file();
};


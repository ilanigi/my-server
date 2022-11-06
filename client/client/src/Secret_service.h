#include <rsa.h>
#include <osrng.h> 
#define KEY_SIZE 1024
#define KEY_SIZE_NET 160

class Secret_service {
public: 
		Secret_service();
	  ~Secret_service();
	  char * get_public_key(char* buffer, unsigned int size) const;
	  std::string decrypt(const char* cipher, unsigned int length);
private:
	std::string get_private_key() const;
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey private_key;
	CryptoPP::RSA::PublicKey public_key;
	void add_key_to_file();
	void decode_key(std::string );
};


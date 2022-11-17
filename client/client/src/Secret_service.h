#include <rsa.h>
#include <osrng.h> 
#define KEY_SIZE_NET 160

class Secret_service {
public: 
		Secret_service();
	  ~Secret_service();
	  void get_public_key(char* keyout, unsigned int length) const;
	  std::string decrypt(const char* cipher, unsigned int length);
	  static uint32_t check_sum(const char* [], size_t );
private:
	static const unsigned int BITS = 1024;
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey private_key;
	void init_from_file(std::string );
	void init_public_key();
	std::string get_private_key() const;
	void add_key_to_file();
};


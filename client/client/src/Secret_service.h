#include <rsa.h>
#include <osrng.h> 
#define KEY_SIZE 1024
class Secret_service {
public: 
		Secret_service();
	  ~Secret_service();
	  std::string get_public_key();
	  std::string get_private_key();
private:
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSA::PrivateKey private_key;
	CryptoPP::RSA::PublicKey public_key;


};


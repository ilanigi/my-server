#include "Secret_service.h"
#include "File_service.h"
#include "Converters.h"
#include <boost/crc.hpp >	
#include <modes.h>



Secret_service::Secret_service() { }
Secret_service::~Secret_service() {}

void Secret_service::init() {
	std::string base_64_private_key;

	if (File_service::file_exist(USER_FILE)) {
		base_64_private_key = File_service::get_private_key();
		if (base_64_private_key.length() > 0) {
			std::cout << "private key found in me.info file." << std::endl;
			init_from_file(base_64_private_key);
		}
		else {
			// create and add to me.info
			std::cout << "crating private key" << std::endl;
			private_key.Initialize(rng, BITS);
			add_key_to_file();
		}
		isInit = true;
	}
	else {
		std::cout << INIT_MESSAGE << std::endl;
	}
}

void Secret_service::add_key_to_file() {
	std::string base_64_key = Base_64_service::encode(get_private_key());
	File_service::add_line_to_file(USER_FILE, base_64_key);
}

void Secret_service::init_from_file(std::string base_64_key) {
	std::string key = Base_64_service::decode(base_64_key);
	CryptoPP::StringSource ss(key, true);
	private_key.Load(ss);
}

void Secret_service::get_public_key(char* keyout, unsigned int length) const
{
	if (!isInit) {
		throw std::invalid_argument(INIT_MESSAGE);
	}
	CryptoPP::RSAFunction publicKey(private_key);
	CryptoPP::ArraySink as(reinterpret_cast<CryptoPP::byte*>(keyout), length);
	publicKey.Save(as);
	return;
}

std::string Secret_service::get_private_key() const {
	std::string key;
	CryptoPP::StringSink ss(key);
	private_key.Save(ss);
	return key;
}

void Secret_service::decrypt_key(const char* cipher, unsigned int length, unsigned char * buffer, unsigned int buffer_size) {
	if (!isInit) {
		throw std::invalid_argument(INIT_MESSAGE);
	}
	std::string decrypted;
	CryptoPP::RSAES_OAEP_SHA_Decryptor d(private_key);
	CryptoPP::StringSource ss_cipher(reinterpret_cast<const CryptoPP::byte*>(cipher), length, true, new CryptoPP::PK_DecryptorFilter(rng, d, new CryptoPP::StringSink(decrypted)));
	memcpy_s(buffer, buffer_size, decrypted.data(), decrypted.length());
}

uint32_t Secret_service::check_sum(const char * file[], size_t size) {
	boost::crc_32_type result;
	result.process_bytes(file, size);
	return result.checksum();
}

std::string Secret_service::encrypt(const char* plain, unsigned int length)
{
	if (!isInit) {
		throw std::invalid_argument(INIT_MESSAGE);
	}
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };
	CryptoPP::AES::Encryption aesEncryption(AES_key, AES_KEY_SIZE);

	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	std::string cipher;
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipher));
	stfEncryptor.Put(reinterpret_cast<const CryptoPP::byte*>(plain), length);
	stfEncryptor.MessageEnd();

	return cipher;
}

void Secret_service::set_AES_key(unsigned char * buffer) {
	memcpy_s(AES_key, AES_KEY_SIZE,buffer, AES_KEY_SIZE);
}

/*
std::string Secret_service::decrypt(const char* cipher, unsigned int length)
{
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!

	CryptoPP::AES::Decryption aesDecryption(_key, DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	std::string decrypted;
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decrypted));
	stfDecryptor.Put(reinterpret_cast<const CryptoPP::byte*>(cipher), length);
	stfDecryptor.MessageEnd();

	return decrypted;
}
*/
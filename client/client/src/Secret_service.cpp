#include "Secret_service.h"
#include "File_service.h"
#include "Converters.h"
#include <boost/crc.hpp >	
#include <modes.h>
#include <files.h>
#include <fstream>




Secret_service::Secret_service() {
	Secret_service::init();
}
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
			// create and add key to me.info
			std::cout << "crating private key" << std::endl;
			private_key.Initialize(rng, BITS);
			if (!private_key.Validate(rng, 2)) {
				throw std::runtime_error("generated key is invalid");
			}
			add_key_to_file();
		}
		isInit = true;
	}
	else {
		std::cout << INIT_MESSAGE << std::endl;
	}
}

void Secret_service::add_key_to_file() {
	std::string str_private_key = get_private_key();
	std::string base_64_key = Base_64_service::encode(str_private_key);
	File_service::add_line_to_file(USER_FILE, base_64_key);
}

void Secret_service::init_from_file(std::string base_64_key) {
	std::string key = Base_64_service::decode(base_64_key);
	CryptoPP::StringSource ss(key, true);
	private_key.Load(ss);
	if (!private_key.Validate(rng, 2)) {
		throw std::runtime_error("loaded key is invalid");
	}
}

std::string Secret_service::get_public_key() const
{
	if (!isInit) {
		throw std::invalid_argument(INIT_MESSAGE);
	}
	CryptoPP::RSAFunction publicKey(private_key);

	std::string key;
	CryptoPP::StringSink ss(key);
	publicKey.Save(ss);
	return key;
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

uint32_t Secret_service::check_sum(std::string file_path) {

	boost::crc_32_type result;

	std::ifstream file(file_path, std::ios_base::binary);

	if (file)
	{
		do
		{
			char  buffer[BUFFER_SIZE];
			file.read(buffer, BUFFER_SIZE);
			result.process_bytes(buffer, file.gcount());
		} while (file);

	}
	else
	{
		throw std::runtime_error("Filed to open file");
	}
	return result.checksum();
}
// TODO: add delete crypted file
std::string Secret_service::encrypt_file(std::string file_name)
{
	if (!isInit) {
		throw std::invalid_argument(INIT_MESSAGE);
	}
	if (!AES_key[0]) {
		throw std::invalid_argument("invalid AES key");
	}
	
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };
	CryptoPP::AES::Encryption aesEncryption(AES_key, AES_KEY_SIZE);

	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
	std::string out_file_name = file_name;

	out_file_name.append("_encrypted");

	std::ifstream in{ file_name, std::ios::binary };
	std::ofstream out{ out_file_name, std::ios::binary };

	CryptoPP::FileSource fileSource(in, true, new CryptoPP::StreamTransformationFilter(cbcEncryption, new CryptoPP::FileSink(out)));

	return out_file_name;
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
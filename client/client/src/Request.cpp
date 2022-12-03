#include "Request.h"

Resquest::Resquest() {}
Resquest::~Resquest() {}

SendKeyRequest::SendKeyRequest(std::vector<char> client_id,std::string public_key) {
	memcpy_s(body.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
	memcpy_s(body.data.public_key, Secret_service::PUBLIC_KEY_SIZE_NET, public_key.data(), public_key.size());
}
const char * SendKeyRequest::getBuffer() {
	return body.buff;
}
size_t SendKeyRequest::getSize() {
	return sizeof(send_key_struct);
}

SendKeyRequest::~SendKeyRequest(){}

SendFileRequest::SendFileRequest(std::string fileName, std::string encrypted_file_name,std::vector<char> client_id, uint32_t file_size)  {
	memcpy_s(body.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
	memcpy_s(body.data.file_name, NAME_MAX_SIZE, fileName.data(), fileName.size());
	body.data.file_size = file_size;
	encypt_file_name = encrypted_file_name;
}
SendFileRequest::~SendFileRequest(){}

const char* SendFileRequest::getBuffer() {
	return body.buff;
}

size_t SendFileRequest::getSize() {
	return sizeof(send_file_struct) + body.data.file_size;
}
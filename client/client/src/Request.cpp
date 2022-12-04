#include "Request.h"
#include "models.h"
#include "File_service.h"

Resquest::Resquest() {}
Resquest::~Resquest() {}

SendKeyRequest::SendKeyRequest(std::vector<char> user_name,std::string public_key) {
	memcpy_s(body.data.name,	NAME_MAX_SIZE, user_name.data(), NAME_MAX_SIZE);
	memcpy_s(body.data.public_key, Secret_service::PUBLIC_KEY_SIZE_NET, public_key.data(), Secret_service::PUBLIC_KEY_SIZE_NET);
}
SendKeyRequest::~SendKeyRequest(){}

const std::vector<char >SendKeyRequest::getParsedRequest() {
	std::vector<char> full_body(std::begin(body.buff), std::end(body.buff));
	return full_body;
}


SendFileRequest::SendFileRequest(std::string file_name, std::string encrypted_file_name,std::vector<char> client_id, uint32_t file_size)  {

	memcpy_s(body.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
	memcpy_s(body.data.file_name, NAME_MAX_SIZE, file_name.data(), file_name.size());
	body.data.content_size = file_size;
	encypt_file_name = encrypted_file_name;

}
SendFileRequest::~SendFileRequest(){}

const std::vector<char> SendFileRequest::getParsedRequest() {
	
	std::vector<char> full_body(std::begin(body.buff), std::end(body.buff));
	std::vector<char> file_content = File_service::get_file_content(encypt_file_name);
	full_body.insert(full_body.end(), file_content.begin(), file_content.end());
	return full_body;

}

AccFile::AccFile(std::string file_name, std::vector<char> client_id) {
	memcpy_s(body.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
	memcpy_s(body.data.file_name, NAME_MAX_SIZE, file_name.data(), file_name.size());
}
AccFile::~AccFile(){}

const std::vector<char> AccFile::getParsedRequest() {
	std::vector<char> body_vector(std::begin(body.buff), std::end(body.buff));
	return body_vector;
}

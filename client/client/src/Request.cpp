#include "Request.h"
#include "File_service.h"

Resquest::Resquest() {}
Resquest::~Resquest() {}

SendKeyRequest::SendKeyRequest(std::vector<char> client_id,std::string public_key) {
	memcpy_s(body.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
	memcpy_s(body.data.public_key, Secret_service::PUBLIC_KEY_SIZE_NET, public_key.data(), public_key.size());
}
SendKeyRequest::~SendKeyRequest(){}

const std::vector<char >SendKeyRequest::getParsedRequest() {
	std::vector<char> full_body(std::begin(body.buff), std::end(body.buff));
	return full_body;
}


SendFileRequest::SendFileRequest(std::string fileName, std::string encrypted_file_name,std::vector<char> client_id, uint32_t file_size)  {

	memcpy_s(body.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
	memcpy_s(body.data.file_name, NAME_MAX_SIZE, fileName.data(), fileName.size());
	body.data.file_size = file_size;
	encypt_file_name = encrypted_file_name;

}
SendFileRequest::~SendFileRequest(){}

const std::vector<char> SendFileRequest::getParsedRequest() {
	
	std::vector<char> full_body(std::begin(body.buff), std::end(body.buff));
	std::vector<char> file_content = File_service::get_file_content(encypt_file_name);
	full_body.insert(full_body.end(), file_content.begin(), file_content.end());
	return full_body;

}

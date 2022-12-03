#include "models.h"

#pragma pack(push,1)
struct send_key_struct {
	uint8_t client_id[NAME_MAX_SIZE];
	uint8_t public_key[Secret_service::PUBLIC_KEY_SIZE_NET];
};
#pragma pack(pop)

union send_key_union {
	send_key_struct data;
	char buff[sizeof(send_key_struct)];
};

#pragma pack(push,1)
struct send_file_struct {
	uint8_t client_id[CLIENT_ID_SIZE];
	uint8_t file_name[NAME_MAX_SIZE];
	uint32_t file_size;
};
#pragma pack(pop)

union send_file_union {
	send_file_struct data;
	char buff[sizeof(send_file_struct)];
};


class Resquest  {
public:
	Resquest();
	~Resquest();
	virtual const std::vector<char> getParsedRequest();	
};

class SendKeyRequest :Resquest{
public:
	SendKeyRequest( std::vector<char> client_id, std::string public_key);
	~SendKeyRequest();
	const std::vector<char> getParsedRequest();
private:
	send_key_union body = {0};;
};


class SendFileRequest :Resquest{
public:
	SendFileRequest(std::string fileName, std::string encrypted_file_name, std::vector<char> client_id, uint32_t file_size);
	~SendFileRequest();
	const std::vector<char> getParsedRequest();
	std::string encypt_file_name;
private:

	send_file_union body = { 0 };;
};


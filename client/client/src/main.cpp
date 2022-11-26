#include "Client.h"
#include <boost/asio.hpp>

int main() {
    
    Client::register_user();
    unsigned char AES_key[Secret_service::AES_KEY_SIZE] = {0};
    Client::create_RSA_keys(AES_key);
    Client::send_file(AES_key);

    // when user exist - should return ...
    // char req[BUFF_SIZE] = "hi";
    // boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}
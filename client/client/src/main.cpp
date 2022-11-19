#include "Client.h"
#include <boost/asio.hpp>

int main() {
    
    Client::register_user();
    Client::create_RSA_keys();
    Client::send_file();
    // when user exist - should return ...
    // char req[BUFF_SIZE] = "hi";
    // boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}
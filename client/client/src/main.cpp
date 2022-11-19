#include "Client.h"
#include <boost/asio.hpp>

int main() {
    Client client;
    client.register_user();
    client.create_RSA_keys();
    client.send_file();
    // when user exist - should return ...
    // char req[BUFF_SIZE] = "hi";
    // boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}
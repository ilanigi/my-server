#include "Client.h"
#include <boost/asio.hpp>

int BUFF_SIZE = 1024;

int main() {
    Client client;
    //register user
    //when user not exist - should create one
     client.register_user();
    client.create_RSA_keys();
    // when user exist - should return ...
    // char req[BUFF_SIZE] = "hi";
    // boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}
#include "Client.hpp"
#include <boost/asio.hpp>

int BUFF_SIZE = 1024;

int main(){
    
    Client client;
    // register user
    // when user not exist - should create one
    client.register_user();
    // when user exist - should return ...
    // char req[BUFF_SIZE] = "hi";
    // boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}

#include "Client.h"
#include <boost/asio.hpp>

int BUFF_SIZE = 1024;

int main(){
    
    Client client;
    char req[BUFF_SIZE] = "hi";
    // boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}

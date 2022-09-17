#include <boost/asio.hpp>

int BUFF_SIZE = 1024;

int main(){
    std::string address = "127.0.0.1";
    std::string port = "8080";
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket client_socket(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(client_socket,resolver.resolve(address,port));
    char req[BUFF_SIZE] = "hi";
    boost::asio::write(client_socket, boost::asio::buffer(req,BUFF_SIZE));
    return 0;
}

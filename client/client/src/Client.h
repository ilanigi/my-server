#pragma once
#include <boost/asio.hpp>

#define TRANSFER_FILE "transfer.info"

class Client {
public:
    Client();
    ~Client();
    void register_user();
    /*const boost::asio::ip::tcp::socket& get_socket();*/

private:    
    boost::asio::io_context client_io_context;
    boost::asio::ip::tcp::socket client_socket;
    static std::vector<std::string> get_connection_credentials();
    void init_connection(std::string, std::string);
    bool user_file_exist();
    std::string get_user_name_from_file();
};



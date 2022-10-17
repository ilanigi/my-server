#include <boost/asio.hpp>
#include <memory>

#define TRANSFER_FILE "transfer.info"

class Client {
public:
    Client();
    ~Client();
    void register_user();
    boost::asio::ip::tcp::socket get_socket();

private:    
    static std::vector<std::string> get_connection_credentials();
    void init_connection(std::string, std::string);
    bool user_file_exist();
    std::string get_user_name_from_file();

    Pre_connection pre_connection_;
};


class Pre_connection {
public:
        Pre_connection();
        ~Pre_connection();
        boost::asio::io_context client_io_context;
        boost::asio::ip::tcp::socket client_socket;
};


#pragma once
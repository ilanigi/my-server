#pragma once
#include <boost/asio.hpp>
#define AES_KEY_SIZE 16
#define ENCRYPTED_AES_KEY_SIZE 128

class Client {
public:
    Client();
    ~Client();
    void register_user();
    void create_RSA_keys();
    std::string AES_key;

private:
    std::string user_name;
    boost::asio::io_context client_io_context;
    boost::asio::ip::tcp::socket client_socket;
};



#pragma once
#include <boost/asio.hpp>
#include "Secret_service.h"

#define ENCRYPTED_AES_KEY_SIZE 128

class Client {
public:
    Client();
    ~Client();
    void register_user();
    void create_RSA_keys();
    void send_file();
private:
    std::string user_name;
    boost::asio::io_context client_io_context;
    boost::asio::ip::tcp::socket client_socket;
    Secret_service secret_service;
};



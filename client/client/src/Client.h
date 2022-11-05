#pragma once
#include <boost/asio.hpp>


class Client {
public:
    Client();
    ~Client();
    void register_user();
    void create_RSA_keys();

private:
    std::string user_name;
    boost::asio::io_context client_io_context;
    boost::asio::ip::tcp::socket client_socket;
};



#include <boost/asio.hpp>
#define TRANSFER_FILE "transfer.info"

class Client {
public:
    Client();
    ~Client();
    void register_user();
private:
    bool user_file_exist();
    std::string get_user_name_from_file();
    boost::asio::ip::tcp::socket* socket;
    static boost::asio::ip::tcp::socket init_connection(std::string, std::string);
    static std::vector<std::string> get_connection_credentials();
};
#pragma once

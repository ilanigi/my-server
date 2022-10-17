#include "Client.h"
#include "models.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <memory>


Pre_connection::Pre_connection() : client_socket(client_io_context) {}
Pre_connection::~Pre_connection() {
    this->client_socket.close();
}

Client::Client()
{
    std::vector<std::string> connection_credentials = this->get_connection_credentials();
    boost::asio::ip::tcp::resolver resolver(pre_connection_.client_io_context);
    boost::asio::connect(pre_connection_.client_socket, resolver.resolve(connection_credentials[0], connection_credentials[1]));
    std::cout << "Server is connected." << std::endl;
}


boost::asio::ip::tcp::socket Client::get_socket() {
    return this->pre_connection_.client_socket;
}





Client::~Client()
{
    std::cout << "Server is disconnected." << std::endl;
}



std::vector<std::string> Client::get_connection_credentials()
{
    std::string address;
    std::string port;

    std::ifstream transfer_file(TRANSFER_FILE);
    //bool a = transfer_file.good();
    getline(transfer_file, address, ':');
    getline(transfer_file, port, '\n');
    transfer_file.close();

    if (port.length() < 1 || address.length() < 1)
    {
        throw std::invalid_argument("transfer.info file is missing port or address");
    }
    std::vector<std::string> to_ret{ address, port };
    return to_ret;
}

bool Client::user_file_exist()
{
    std::string info_file_name = "me.info";
    std::ifstream info_file(info_file_name);
    return info_file.good();
}

std::string Client::get_user_name_from_file()
{
    std::string buff;
    std::string user_name;

    std::ifstream transfer_file(TRANSFER_FILE);
    getline(transfer_file, buff, '\n');
    getline(transfer_file, user_name, '\n');

    buff.clear();
    return user_name;
}

void Client::register_user()
{
    if (this->user_file_exist())
    {
        std::cout << "User credentials already exist." << std::endl;
        return;
    }

    std::cout << "Register user" << std::endl;

    std::string user_name = this->get_user_name_from_file();
    res_header header = { 0 };
    header.data.code = REQ_CODE::REGISTER;
    header.data.version = CLIENT_VERSION;
    header.data.payload_size = REQ_PAYLOAD_SIZE::REGISTER_S;

    char req[REQ_PAYLOAD_SIZE::REGISTER_S + sizeof(res_header)] = { 0 };
    int i = 0
        ;
    while (i < sizeof(res_header))
    {
        req[i] = header.buff[i];
        i++;
    }
    
    for (auto letter : user_name)
    {
        req[i++] = letter;
    }

    boost::asio::write(this->get_socket(), boost::asio::buffer(req, REQ_PAYLOAD_SIZE::REGISTER_S + sizeof(res_header)));
}

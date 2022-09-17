#include "Client.h"
#include "utils.h"

#include <string>
#include <stdexcept>
#include <boost/asio.hpp>
#include <fstream>
#pragma once

Client::Client()
{

  std::vector<std::string> connection_credentials = this->get_connection_credentials();
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::socket client_socket(io_context);
  boost::asio::ip::tcp::resolver resolver(io_context);
  this->socket = &client_socket;
  boost::asio::connect(client_socket, resolver.resolve(connection_credentials[0], connection_credentials[1]));
}
Client::~Client()
{
  this->socket->close();
}

std::vector<std::string> Client::get_connection_credentials()
{
  std::string address;
  std::string port;

  std::ifstream transfer_file(TRANSFER_FILE);
  getline(transfer_file, address, ':');
  getline(transfer_file, port, '\n');
  transfer_file.close();

  if (port.length() < 1 || address.length() < 1)
  {
    throw std::invalid_argument("transfer.info file is missing port or address");
  }
  std::vector<std::string> to_ret{address, port};
  return to_ret;
}
bool Client::user_file_exist()
{
  std::string info_file_name = "me.info";
  std::ifstream info_file(info_file_name);
  return info_file.good();
}
std::string get_user_name_from_file()
{
  std::string buff;
  std::string user_name;
  std::ifstream transfer_file(TRANSFER_FILE);
  getline(transfer_file, buff, '\n');
  getline(transfer_file, user_name, '\n');
}

void Client::register_user()
{
  if (this->user_file_exist())
  {
    print("user credentials already exist.");
    return;
  }
}

#include "Client.h"
#include "utils.h"
#include <string>
#include <boost/asio.hpp>
#include <fstream>
#pragma once

Client::Client()
{
  std::vector<std::string> connection_cradentials = this->get_connection_credentials();
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::socket client_socket(io_context);
  boost::asio::ip::tcp::resolver resolver(io_context);
  this->socket = &client_socket;
  try
  {
    boost::asio::connect(client_socket, resolver.resolve(connection_cradentials[0], connection_cradentials[1]));
  }
  catch (const std::exception &e)
  {
    print("Failed connect to serve:");
    print(e.what());
    exit(-1);
  }
}
Client::~Client()
{
  this->socket->close();
}

std::vector<std::string> Client::get_connection_credentials()
{
  std::string address;
  std::string port;
  const std::string transfer_path = "transfer.info";
  try
  {
    std::ifstream transfer_file(transfer_path);
    getline(transfer_file, address, ':');
    getline(transfer_file, port, '\n');
    transfer_file.close();
  }
  catch (const std::exception &e)
  {
    print("Failed to open transfer.info file");
  }
  if (port.length() < 1 || address.length() < 1)
  {
    print("transfer.info file is missing port or address");
  }
  std::vector<std::string> to_ret{address, port};
  return to_ret;
}
void Client::register_user()
{
  if (this->user_file_exist())
  {
    print("user credentials already exist.");
    return;
  }
  
  
}

bool Client::user_file_exist()
{
  std::string info_file_name = "me.info";
  std::ifstream info_file(info_file_name);
  return info_file.good();
}
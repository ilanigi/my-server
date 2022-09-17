#include "Client.h"
#include <boost/asio.hpp>
#include <fstream>
#pragma once


Client::Client()
{
  std::vector<std::string> connection_cradentials = this->get_connection_cradentilas();
  Client::init_connection(connection_cradentials[0], connection_cradentials[1]);
}

boost::asio::ip::tcp::socket Client::init_connection(std::string address, std::string port)
{
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::socket client_socket(io_context);
  boost::asio::ip::tcp::resolver resolver(io_context);
  boost::asio::connect(client_socket, resolver.resolve(address, port));
  return client_socket;
}

std::vector<std::string> Client::get_connection_cradentilas()
{
  std::string address;
  std::string port;
  const std::string tranfer_path = "transfer.info";
  try
  {
    std::ifstream transfer_file(tranfer_path);
    getline(transfer_file, address,':');
    getline(transfer_file, port, '\n');
    transfer_file.close();
  }
  catch (const std::exception &e)
  {
    // todo: add print
  }
  if(port.length() < 1 || address.length()< 1){
  
  }
  std::vector<std::string> to_ret{address, port};
  return to_ret;
}

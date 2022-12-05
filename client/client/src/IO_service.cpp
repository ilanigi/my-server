#include "IO_service.h"
#include "File_service.h"
#include "models.h"

#include <fstream>
#include <iostream>
#include <boost/bind.hpp>

Connection::Connection() :resolver(io_service), client_socket(io_service) {}
Connection::~Connection() {
    
}

IO_service::IO_service(): connection() , request() {}
IO_service::~IO_service(){
    
}

void IO_service::connect() {
  
    std::vector<std::string> connection_credentials = File_service::get_connection_credentials();
    boost::asio::ip::tcp::resolver::query query(connection_credentials[0], connection_credentials[1]);
    
    connection.resolver.async_resolve(query,
        boost::bind(&IO_service::handle_resolve, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::iterator));
   
}

void IO_service::handle_resolve(const boost::system::error_code& err , boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    if (!err)
    {
        boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
        connection.client_socket.async_connect(endpoint,
            boost::bind(&IO_service::handle_connect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void IO_service::handle_connect(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
    if (!err) {
        boost::asio::async_write(connection.client_socket, request,
            boost::bind(&IO_service::handle_write_request, this,
                boost::asio::placeholders::error));
        
    }
    else{
        if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
        {
            connection.client_socket.close();
            boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
            connection.client_socket.async_connect(endpoint,
                boost::bind(&IO_service::handle_connect, this,
                    boost::asio::placeholders::error, ++endpoint_iterator));
        }
        else
        {
            throw  std::invalid_argument(err.message());   
        }
    }
}

void IO_service::handle_write_request(const boost::system::error_code& err) {
     if (!err)
    {
         if(!skip_res){
            boost::asio::async_read(connection.client_socket, header_buf, boost::asio::transfer_exactly(sizeof(res_header)),
            boost::bind(&IO_service::handle_read_headers, this,
                boost::asio::placeholders::error));
         }
         else {
             handle_read_body(err);
         }
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }

}

void IO_service::handle_read_headers(const boost::system::error_code& err) {
    std::istream is(&header_buf);
    is.read(header_strct.buff, sizeof(res_header));

    if (!err)
    {
        if (!header_strct.data.code)
        {
            std::cout << "Invalid response\n";
            return;
        }
        if (header_strct.data.payload_size > 0) {
            boost::asio::async_read(connection.client_socket, response, boost::asio::transfer_exactly(header_strct.data.payload_size),
            boost::bind(&IO_service::handle_read_body, this,
                boost::asio::placeholders::error));
        }
        handle_read_body(err);
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

void IO_service::handle_read_body(const boost::system::error_code& err) {

    if (!err)
    {
       
        finish_wait();
        connection.client_socket.close();
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

void IO_service::send(unsigned int req_code, size_t payload_size, std::vector<char> payload, std::vector<char> client_id) {
    skip_res = req_code == REQ_CODE::CRC_FAILED || req_code == REQ_CODE::CRC_INVALID;
    //create busy wating
    start_wait();
        
    //set header and body
    req_header header = { 0 };
    
    if (client_id.size() > 0) {
        memcpy_s(header.data.client_id, CLIENT_ID_SIZE,client_id.data(), CLIENT_ID_SIZE);
    }

    header.data.code = req_code;
    header.data.version = CLIENT_VERSION;
    header.data.payload_size = payload_size;

    std::size_t req_size = payload_size + sizeof(req_header);
    std::vector<char> req(req_size);

    memcpy_s(req.data(), sizeof(req_header), header.buff, sizeof(req_header));
    memcpy_s(req.data() + sizeof(req_header), payload_size, payload.data(), payload_size);
    std::ostream(&request).write(req.data(), req_size);
   
    connect();
    connection.io_service.restart();
    connection.io_service.run();

}


//void IO_service::send_file( std::string encrypted_file_content, std::vector<char> client_id) {
//    //create busy wating
//    start_wait();
//
//    req_header header = { 0 };
//    
//    memcpy_s(header.data.client_id, CLIENT_ID_SIZE, client_id.data(), client_id.size());
//    
//    header.data.code = ;
//    header.data.version = CLIENT_VERSION;
//    header.data.payload_size = encrypted_file_content.size();
//
//    std::size_t req_size = encrypted_file_content.size() + sizeof(req_header);
//    std::vector<char> req(req_size);
//
//    memcpy_s(req.data(), sizeof(req_header), header.buff, sizeof(req_header));
//
//}
uint16_t IO_service::get_res_status()  {
    return header_strct.data.code;
}

void IO_service::start_wait() {
    wait = true;
}
void IO_service::do_wait() const {
    while (wait) {
        ;
    }
}
void IO_service::finish_wait() {
    wait = false;

}

boost::asio::streambuf* IO_service::get_response_body(){
    return &response;
}


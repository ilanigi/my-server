#include "IO_service.h"
#include "File_service.h"
#include "models.h"

#include <iostream>
#include <boost/bind.hpp>

Connection::Connection() :resolver(io_service), client_socket(io_service) {}
Connection::~Connection() {
    client_socket.close();
}

IO_service::IO_service(): connection() , request() {}
IO_service::~IO_service(){
    
}

void IO_service::connect() {
      try
    {
        std::vector<std::string> connection_credentials = File_service::get_connection_credentials();
        boost::asio::ip::tcp::resolver::query query(connection_credentials[0], connection_credentials[1]);
                //boost::asio::ip::tcp::resolver resolver(client_io_context);
        /*boost::asio::connect(client_socket, resolver.resolve(connection_credentials[0], connection_credentials[1]));*/

        connection.resolver.async_resolve(query,
            boost::bind(&IO_service::handle_resolve, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator));

    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed connect to serve:" << '\n';
        std::cerr << e.what() << '\n';
        exit(-1);
    }
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
            std::cout << "Error: " << err.message() << "\n";
        }
    }
    std::cout << "Server is connected." << std::endl;
}

void IO_service::handle_write_request(const boost::system::error_code& err) {

    if (!err)
    {
        boost::asio::async_read(connection.client_socket, header_buf, boost::asio::transfer_exactly(sizeof(res_header)),
            boost::bind(&IO_service::handle_read_headers, this,
                boost::asio::placeholders::error));
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
            boost::asio::async_read(connection.client_socket, response, boost::asio::transfer_exactly(sizeof(header_strct.data.payload_size)),
            boost::bind(&IO_service::handle_read_body, this,
                boost::asio::placeholders::error));
        }
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

void IO_service::handle_read_body(const boost::system::error_code& err) {
    if (!err)
    {
        // Process the response headers.
        std::istream response_stream(&response);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";

        // Write whatever content we already have to output.
        if (response.size() > 0)
            std::cout << &response;

        // Start reading remaining data until EOF.
        boost::asio::async_read(connection.client_socket, response,
            boost::asio::transfer_at_least(1),
            boost::bind(&IO_service::finish_wait, this));
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

//void IO_service::handle_read_content(const boost::system::error_code& err) {
//    boost::asio::streambuf response;
//
//    if (!err)
//    {
//        // Write all of the data that has been read so far.
//        std::cout << &response;
//
//        // Continue reading remaining data until EOF.
//        boost::asio::async_read(socket, response,
//            boost::asio::transfer_at_least(1),
//            boost::bind(&IO_service::handle_read_content, this,
//                boost::asio::placeholders::error));
//    }
//    else if (err != boost::asio::error::eof)
//    {
//        std::cout << "Error: " << err << "\n";
//    }
//}

void IO_service::send(unsigned int req_code, size_t payload_size, std::string payload) {
    boost::asio::streambuf response;
    req_header header = { 0 };

    header.data.code = req_code;
    header.data.version = CLIENT_VERSION;
    header.data.payload_size = payload_size;

    std::size_t req_size = payload_size + sizeof(req_header);

    std::vector<char> req(req_size);

    memcpy_s(req.data(), sizeof(req_header), header.buff, sizeof(req_header));
    memcpy_s(req.data() + sizeof(req_header), payload_size, payload.c_str(), payload_size);
    
    std::ostream(&request).write(req.data(), req_size);

    connect();
    connection.io_service.run();

}

uint16_t IO_service::get_res_status()  {
    return header_strct.data.code;
}

void IO_service::start_wait() {
    wait = true;
}
bool IO_service::should_wait() const {
    return wait;
}
void IO_service::finish_wait() {
    wait = false;

}
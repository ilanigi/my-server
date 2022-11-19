#pragma once
#include "models.h"
#include <boost/asio.hpp>

class Connection {
public:
	Connection();
	~Connection();
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket client_socket;
	boost::asio::ip::tcp::resolver resolver;

};

class IO_service {
public:
	IO_service();
	~IO_service();
	void send(unsigned int , size_t , std::string);
	uint16_t get_res_status();
	void start_wait();
	bool should_wait() const;
	boost::asio::streambuf request;

private:
	Connection connection;

	void connect();

	bool wait = true;
	static const unsigned int CLIENT_VERSION = 3;
	boost::asio::streambuf response;
	boost::asio::streambuf header_buf;
	res_header header_strct;

    
	void finish_wait();
	

	void handle_resolve(const boost::system::error_code& , boost::asio::ip::tcp::resolver::iterator );
	void handle_connect(const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator);
	void handle_write_request(const boost::system::error_code&);
	void handle_read_headers(const boost::system::error_code& );
	void handle_read_body(const boost::system::error_code& err);
	//void handle_read_content(const boost::system::error_code& );
	//void handle_read_status_line(const boost::system::error_code& );



};


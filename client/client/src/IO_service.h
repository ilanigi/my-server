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
	boost::asio::streambuf request;
	IO_service();
	~IO_service();
	void send(unsigned int req_code, size_t payload_size, std::string payload, std::vector<char> client_id);
	//void send_file( std::string encrypted_file_content, std::vector<char> client_id);

	bool should_wait() const;
	boost::asio::streambuf* get_response_body();
	uint16_t get_res_status();

private:
	void start_wait();
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



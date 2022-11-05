#include "Client.h"
#include "models.h"
#include "Secret_service.h"
#include "File_service.h"


#include <vector>
#include <string>
#include <stdexcept>
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>

Client::Client() : client_socket(client_io_context)
{
    try
    {
        std::vector<std::string> connection_credentials = File_service::get_connection_credentials();
        boost::asio::ip::tcp::resolver resolver(client_io_context);
        boost::asio::connect(client_socket, resolver.resolve(connection_credentials[0], connection_credentials[1]));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed connect to serve:" << '\n';
        std::cerr << e.what() << '\n';
        exit(-1);
    }

    std::cout << "Server is connected." << std::endl;
}

Client::~Client()
{
    client_socket.close();
    std::cout << "Server is disconnected." << std::endl;
}


void Client::register_user()
{
    if (File_service::file_exist(USER_FILE))
    {
        std::cout << "User credentials already exist." << std::endl;
        return;
    }

    std::cout << "Register user" << std::endl;
    //TODO: add check  - max user name size is 255 chars
    user_name = get_user_name_from_file();
    req_header header = { 0 };
    header.data.code = REQ_CODE::REGISTER;
    header.data.version = CLIENT_VERSION;
    header.data.payload_size = user_name.length();

    std::size_t req_size = user_name.length() + sizeof(req_header);

    char * req = new char[req_size]();

    int i = 0;
    while (i < sizeof(req_header))
    {
        req[i] = header.buff[i];
        i++;
    }

    for (auto letter : user_name)
    {
        req[i++] = letter;
    }

    boost::asio::write(client_socket, boost::asio::buffer(req, req_size));

    res_header res_header = { 0 };
    char user_id_buff[USER_ID_SIZE + 1] = { 0 };

    size_t length = boost::asio::read(client_socket, boost::asio::buffer(res_header.buff, sizeof(res_header)));
    if (res_header.data.code == RES_CODE::SUCCESSFUL_REGISTER)
    {
        length = boost::asio::read(client_socket, boost::asio::buffer(user_id_buff, USER_ID_SIZE));
        std::string user_id(user_id_buff);

        std::ofstream outfile("USER_FILE ");
        outfile << user_name << std::endl;
        outfile << user_id << std::endl;
        outfile.close();
    }
    else if (res_header.data.code == RES_CODE::REGISTER_FAILED)
    {
        std::cout << "Failed to register new user:" << std::endl;
        // TODO: create retry system , add first error: 'server responded with an error' and after 3 tries full error
    }
    else
    {
        std::cout << "General error accrued" << std::endl;
    }

}

void Client::create_RSA_keys() {

    /*CryptoPP::RandomNumberGenerator rng;
    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 1024);

    CryptoPP::RSA::PrivateKey privateKey(params);
    CryptoPP::RSA::PublicKey publicKey(params);*/

    Secret_service secret;

    secret.

    

    ////////////////////////////////////////////////////////////////////////////////////

    /*CryptoPP::DSA::PrivateKey dsaPrivate;
    dsaPrivate.GenerateRandomWithKeySize(prng, 1024);

    CryptoPP::DSA::PublicKey dsaPublic;
    dsaPrivate.MakePublicKey(dsaPublic);

    SavePrivateKey("dsa-private.key", dsaPrivate);
    SavePublicKey("dsa-public.key", dsaPublic);*/


    req_header header = { 0 };

    header.data.code = REQ_CODE::SEND_PUBLIC_KEY;
    header.data.version = CLIENT_VERSION;
    //header.data.payload_size = sizeof(publicKey);

    std::size_t req_size = user_name.length() + sizeof(req_header);

    char* req = new char[req_size]();

    int i = 0;
    while (i < sizeof(req_header))
    {
        req[i] = header.buff[i];
        i++;
    }


    
 
}

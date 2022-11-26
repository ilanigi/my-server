#include "Client.h"
#include "models.h"
#include "Secret_service.h"
#include "File_service.h"
#include "Converters.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>

Client::Client() 
{
}

Client::~Client()
{   
}


void Client::register_user() 
{
    try {

        std::cout << "Register user" << std::endl;
        Services services;
        if (File_service::file_exist(USER_FILE))
        {
            std::cout << "User credentials already exist." << std::endl;
            return;
        }

    
        std::string user_name = File_service::get_user_name_from_file();

        if (user_name.length() > MAX_USER_SIZE) {
            std::cout << "User name is too long. Please pick a shorter one." << std::endl;
            return;
        }

        std::vector<char> empty_client_id;
        services.io.send(REQ_CODE::REGISTER, user_name.length(), user_name, empty_client_id);

        while (services.io.should_wait()) {
            ;
        }

        uint8_t client_id_buff[CLIENT_ID_SIZE] = { 0 };

        uint16_t code = services.io.get_res_status();

        if (code == RES_CODE::SUCCESSFUL_REGISTER)
        {
            std::istream is(services.io.get_response_body());
            is.read((char*)client_id_buff, CLIENT_ID_SIZE);

            std::string client_id = bytes_to_hex(client_id_buff, CLIENT_ID_SIZE);

            File_service::add_line_to_file(USER_FILE, user_name);
            File_service::add_line_to_file(USER_FILE, client_id);
            std::cout << "User registered successully" << std::endl;

        }
        else if (code == RES_CODE::REGISTER_FAILED)
        {
            std::cout << "Failed to register new user:" << std::endl;
            // TODO: create retry system , add first error: 'server responded with an error' and after 3 tries full error
        }
        else
        {
            std::cout << "General error accrued" << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Failed register user:" << '\n';
        std::cerr << e.what() << '\n';
    }

}
    
void Client::create_RSA_keys() {
    try{
 
        Services services;
        
        std::string public_key = services.secrets.get_public_key();
        std::vector<char> client_id = File_service::get_client_id();
    
        std::cout << "Sending public key..." << std::endl;
        
        services.io.send(REQ_CODE::SEND_PUBLIC_KEY, Secret_service::PUBLIC_KEY_SIZE_NET, public_key, client_id);
    
        while (services.io.should_wait()) {
            ;
        }

        uint16_t code = services.io.get_res_status();

        if (code == RES_CODE::PUBLIC_KEY_RECEIVED)
        {   
            char encrypet_AES_key_buffer[ENCRYPTED_AES_KEY_SIZE] = { 0 };
            std::istream is(services.io.get_response_body());

            is.read((char*)encrypet_AES_key_buffer, ENCRYPTED_AES_KEY_SIZE);
                
            unsigned char AES_key[Secret_service::AES_KEY_SIZE];

            services.secrets.decrypt_key(encrypet_AES_key_buffer, ENCRYPTED_AES_KEY_SIZE, AES_key, Secret_service::AES_KEY_SIZE);
            services.secrets.set_AES_key(AES_key);

            std::cout << "AES key recived successfully" << std::endl;
        }
        else {
            std::cout << "Failed to send public key" << std::endl;
        }   
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Failed send public key:" << '\n';
        std::cerr << e.what() << '\n';
    }
}

void Client::send_file() {
    Services services;
    std::cout << "Getting file path" << std::endl;
    std::string file_path = File_service::get_file_path();
    
    size_t file_size = File_service::get_file_size(file_path);  
    
    std::string file_content = File_service::get_file_content(file_path);
    std::string message = "this is a secret!";
    std::string encrypted = services.secrets.encrypt(message.c_str(), message.length());

    std::vector<char> client_id = File_service::get_client_id();
        services.io.send(REQ_CODE::SEND_FILE,encrypted.size(),encrypted, client_id);
    
    unsigned int check_sum = Secret_service::check_sum(file_path);
    while (services.io.should_wait()) {
        ;
    }
    

   
}



Services::Services():io(), secrets() {}
Services::~Services(){}

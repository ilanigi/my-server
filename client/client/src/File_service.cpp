#include"File_service.h"
#include "Converters.h"
#include "models.h"

#include <ios>
#include <fstream>
#include <string>
#include <vector>



bool File_service::file_exist(std::string file_name)
{
    std::ifstream file(file_name);
    return file.good();
}

std::vector<std::string> File_service::get_connection_credentials()
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
    std::vector<std::string> to_ret{ address, port };
    return to_ret;
}

std::string File_service::get_user_name_from_file()
{
    std::string buff;
    std::string user_name;

    std::ifstream transfer_file(TRANSFER_FILE);
    getline(transfer_file, buff, '\n');
    getline(transfer_file, user_name, '\n');

    buff.clear();
    return user_name;
}

std::vector<char>  File_service::get_client_id() {
    std::string buff;
    std::string hex_client_id;

    std::ifstream transfer_file(USER_FILE);
    getline(transfer_file, buff, '\n');
    getline(transfer_file, hex_client_id, '\n');


    buff.clear();
    std::vector<char> bytes_client_id = hex_to_bytes(hex_client_id);

    return bytes_client_id;

}

std::string File_service::get_private_key() {

    std::string buff;
    std::string key;
    std::ifstream user_file(USER_FILE);
    getline(user_file, buff, '\n');
    getline(user_file, buff, '\n');
    buff = "";
    while (!user_file.eof()) {
        user_file >> buff;
        key.append(buff);
    }
    user_file.close();
    buff.clear();
    return key;
}

void File_service::add_line_to_file(std::string file_name, std::string line) {
    std::ofstream file(file_name, std::ios_base::app | std::ios_base::out);
    file << line << std::endl;;
    file.close();
}

std::string File_service::get_file_name() {
    std::string buff;
    std::string file_path;

    std::ifstream transfer_file(TRANSFER_FILE);
    getline(transfer_file, buff, '\n');
    getline(transfer_file, buff, '\n');
    getline(transfer_file, file_path, '\n');

    buff.clear();
    return file_path;
}

size_t File_service::get_file_size(std::string file_name) {
    
    std::ifstream file(file_name, std::ios::binary);
    
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    return file_size;


}
//std::vector<char> File_service::get_file_content(std::string file_path) {
//      
//    std::ifstream file (file_path, std::ios_base::binary);
//    if (!file.good()) {
//        throw std::runtime_error("File does not exist");
//    }
//
//    std::vector<char> bytes((std::istreambuf_iterator<char>(file)),
//        (std::istreambuf_iterator<char>()));
//
//    return bytes; 
//    
//    }

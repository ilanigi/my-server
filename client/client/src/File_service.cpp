#include"File_service.h"

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

std::string File_service::get_private_key() {

    std::string buff;
    std::string key;
    std::ifstream user_file(USER_FILE);
    getline(user_file, buff, '\n');
    getline(user_file, buff, '\n');

    getline(user_file, key, '\n');
    user_file.close();

    return key;
}


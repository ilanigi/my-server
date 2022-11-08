#pragma once
#include <string>
#include <vector>

#define TRANSFER_FILE "transfer.info"
#define USER_FILE "me.info"

class File_service {
public:
static bool file_exist(std::string file_name);
static  std::vector<std::string> get_connection_credentials();
static  std::string get_user_name_from_file();
static  std::string get_private_key();
static void add_line_to_file(std::string file_name, std::string line);
static std::vector<char> get_client_id();
static std::string get_file_path();

};

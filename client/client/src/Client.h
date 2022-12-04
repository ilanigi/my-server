#pragma once
#include "Secret_service.h"
#include "IO_service.h"

#define ENCRYPTED_AES_KEY_SIZE 128

class Services {
public:
    Services();
    ~Services();
    IO_service io;
    Secret_service secrets;
};

class Client {
public:
    Client();
    ~Client();
    static void register_user();
    static void create_RSA_keys(unsigned char * AES_key);
    static void send_file(unsigned char* AES_key);
    static const int MAX_USER_NAME_SIZE = 100;
private:
    static const short int MAX_FILE_SEND_RETRIES = 4;
    
};



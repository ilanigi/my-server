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
    static void create_RSA_keys();
    static void send_file();
    static const int MAX_USER_SIZE = 255;
};



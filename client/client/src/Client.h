#pragma once
#include "Secret_service.h"
#include "IO_service.h"

#define ENCRYPTED_AES_KEY_SIZE 128

class Services {
public:
    Services();
    ~Services();
    IO_service io_service;
    Secret_service secret_service;
};

class Client {
public:
    Client();
    ~Client();
    static void register_user();
    static void create_RSA_keys();
    static void send_file();

};



#pragma once

#define CLIENT_ID_SIZE 16
#define CLIENT_ID_STR_SIZE 32
#define CLIENT_VERSION 3

#pragma pack(push,1)
struct req_header_model {
    uint8_t client_id[CLIENT_ID_SIZE];
    uint8_t version;
    uint16_t code;
    uint32_t payload_size;
};
#pragma pack(pop)

union req_header
{
    req_header_model data;
    char buff[sizeof(req_header_model)];
};

enum REQ_CODE {
    REGISTER = 1100,
    SEND_PUBLIC_KEY = 1101,
    SEND_FILE = 1103,
    CRC_VALID = 1104,
    CRC_INVALID = 1105,
    CRC_FAILED = 1106
};

enum REQ_PAYLOAD_SIZE {
    // REGISTER_S = 255,
    SEND_PUBLIC_KEY_S = 415,
    SEND_FILE_S = 275
};

#pragma pack(push,1)
struct res_header_model {
    uint8_t version;
    uint16_t code;
    uint32_t payload_size;
};
#pragma pack(pop)

union res_header
{
    res_header_model data;
    char buff[sizeof(res_header_model)];
};

enum RES_CODE {
    SUCCESSFUL_REGISTER = 2100,
    REGISTER_FAILED = 2101,
    PUBLIC_KEY_RECEIVED = 2102,
    FILE_RECEIVED = 2103,
    MESSAGE_RECEIVED = 2104
};

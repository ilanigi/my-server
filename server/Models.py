from enum import Enum
from struct import unpack_from, pack

REQ_HEADER_FORMAT = "<16sBHI"
RES_FORMAT_BASE = "<BHI"
VERSION = 3
HEADER_SIZE = 23

class REQ_CODE(Enum):
    REGISTER = 1100
    SEND_PUBLIC_KEY = 1101
    SEND_FILE = 1103
    CRC_VALID = 1104
    CRC_INVALID = 1105
    CRC_FAILED = 110

class RES_CODE(Enum):
    ACC_REGISTER = 2100
    REGISTER_FAILED = 2101
    ACC_PUBLIC_KEY = 2102
    ACC_FILE = 2103
    ACC_MESSAGE = 2104



class Request_Header:
    def __init__(self,buffer ) -> None:
        user_id, version, code, payload_size = unpack_from(REQ_HEADER_FORMAT,buffer=buffer,offset=0)
        self.user_id = user_id
        self.version = version
        self.code = code
        self.payload_size = payload_size


class Response:
    def __init__(self, code:RES_CODE, payload) -> None:
        
        RES_FORMAT = RES_FORMAT_BASE
        
        
        if type(payload) == str:
            RES_FORMAT += f'{len(payload)}s'
            payload_size = len(payload)
            payload = bytes(payload ,'utf-8')

        if type(payload) == bytes:
            RES_FORMAT += f'{len(payload)}s'
            payload_size = len(payload)


        self.compiled = pack(RES_FORMAT,VERSION, code, payload_size, payload)



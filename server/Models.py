from enum import Enum
from struct import unpack_from, pack

REQ_HEADER_FORMAT = "<16pBHI"
RES_HEADER_FORMAT = "<BHI"

class REQ_CODE(Enum):
    REGISTER = 1100
    SEND_PUBLIC_KEY = 1101
    SEND_FILE = 1103
    CRC_VALID = 1104
    CRC_INVALID = 1105
    CRC_FAILED = 110

class RES_CODE(Enum):
        REGISTER_ACC = 2100
        REGISTER_FAILED = 2101
        PUBLIC_KEY_ACC = 2102
        FILE_ACC = 2103
        MESSAGE_ACC = 2104



class Header:
    def __init__(self,data ) -> None:
        user_id, version, code, payload_size = unpack_from(REQ_HEADER_FORMAT,buffer=data,offset=0)
        self.user_id = user_id.decode("utf-8") 
        self.version = version
        self.code = code
        self.payload_size = payload_size


class Response:
    def __init__(self, version, code:RES_CODE, payload_size) -> None:
        self.version = version
        self.code = code
        self.payload_size = payload_size
        self.compiled = pack(RES_HEADER_FORMAT,version, code, payload_size)



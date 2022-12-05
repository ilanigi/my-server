from enum import Enum
from struct import unpack_from, pack
NAME_SIZE = 255
REQ_HEADER_FORMAT = "<16sBHI"
RES_FORMAT_BASE = "<BHI"
U_INT_SIZE = 4
VERSION = 3
HEADER_SIZE = 23

class REQ_CODE(Enum):
    REGISTER = 1100
    SEND_PUBLIC_KEY = 1101
    SEND_FILE = 1103
    CRC_VALID = 1104
    CRC_INVALID = 1105
    CRC_FAILED = 1106

class RES_CODE(Enum):
    ACC_REGISTER = 2100
    REGISTER_FAILED = 2101
    ACC_PUBLIC_KEY = 2102
    ACC_FILE = 2103
    ACC_MESSAGE = 2104

class Request_Header:
    def __init__(self,buffer ) -> None:
        client_id, version, code, payload_size = unpack_from(REQ_HEADER_FORMAT,buffer=buffer,offset=0)
        self.client_id = client_id
        self.version = version
        self.code = code
        self.payload_size = payload_size


class Response:
    def __init__(self, code:RES_CODE, RES_FORMAT, payload_size, payload ) -> None:
        TOTAL_RES_FORMAT = RES_FORMAT_BASE + RES_FORMAT
        self.compiled = pack(TOTAL_RES_FORMAT,VERSION, code, payload_size, *payload)

class ACC_REGISTER(Response):
    def __init__(self, client_id) -> None:
        RES_FORMAT = f'{len(client_id)}s'
        super().__init__(RES_CODE.ACC_REGISTER.value, RES_FORMAT, len(client_id), [client_id])

class ACC_PUBLIC_KEY (Response):
    def __init__(self, client_id, AES_key) -> None:
        
        if type(client_id) is not bytes or type(AES_key) is not bytes:
            raise Exception("wrong payload type")
        
        # client id and AES key size are const
        RES_FORMAT = f'{len(client_id)}s{len(AES_key)}s'
        payload_size = len(client_id)+len(AES_key)
        super().__init__(RES_CODE.ACC_PUBLIC_KEY.value, RES_FORMAT,payload_size,(client_id, AES_key))

class ACC_FILE(Response):
    def __init__(self, client_id, content_size, file_name, checksum) -> None:
        RES_FORMAT = f'{len(client_id)}sI{NAME_SIZE}sI'
        payload_size = len(client_id) + U_INT_SIZE*2 + NAME_SIZE
        super().__init__(RES_CODE.ACC_FILE.value, RES_FORMAT, payload_size, (client_id, content_size, bytes(file_name ,'utf-8'), checksum))
    pass

class Empty_response:
    def __init__(self, code:RES_CODE) -> None:
        self.compiled = pack(RES_FORMAT_BASE,VERSION, code, 0)
    
class REGISTER_FAILED(Empty_response):
    def __init__(self) -> None:
        super().__init__(RES_CODE.REGISTER_FAILED.value)
    pass

class ACC_MESSAGE(Empty_response):
    def __init__(self) -> None:
        super().__init__(RES_CODE.ACC_MESSAGE.value)
    pass
        

    





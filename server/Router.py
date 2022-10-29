from struct import unpack_from

HEADER_FORMAT = "<BHH"
class Router:
    def __init__(self):
        pass
    def prase_data(self,data):
        
        header = Header(*unpack_from(HEADER_FORMAT,buffer=data,offset=0))
        pass

class Header:
    def __init__(self,version, code, payload_size ) -> None:
        self.version = version
        self.code = code
        self.payload_size = payload_size
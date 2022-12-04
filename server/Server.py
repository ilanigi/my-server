import selectors
import types    

from Services import Services
from Database import Database
from struct import unpack_from

from Models import HEADER_SIZE, NAME_SIZE, REQ_CODE, Request_Header, U_INT_SIZE
from Controller import Controller
from Connection import Connection



MESSAGE_SIZE = 1024
TIMEOUT = 0
CLIENT_ID_SIZE = 16
PUBLIC_KEY_SIZE = 160

class Server:
    def __init__(self,test=False):
        try:
            db = Database()
            services = Services(db)
            self.services = services 
            self.controller = Controller(services)
            
            if test:
                return 
            self.__connection = Connection()
            self.__listen()

        except Exception as error:
            # TODO: change to res.error(....
            print(error)
        finally:
            if not test:
                self.__connection.selector.close()

    def __listen(self):
        while True:
            events = self.__connection.selector.select(timeout=TIMEOUT)
            for key, mask in events:
                if key.data is None:
                    self.__accept_wrapper(key.fileobj)
                else:
                    self.__service_connection(key, mask)

    def __accept_wrapper(self,sock):
        conn, addr = sock.accept()  
        print(f"Accepted connection from {addr[0]}")
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.__connection.selector.register(conn, events, data=data)

    def __service_connection(self, key, mask):
        self.socket = key.fileobj
        message = key.data
        if mask & selectors.EVENT_READ:
            message_start = self.socket.recv(MESSAGE_SIZE)
            if message_start:
                message.outb = self.handle_request(message_start)
            else:
                print(f"Closing connection to {message.addr[0]}")
                self.__connection.selector.unregister(self.socket)
                self.socket.close()
        if mask & selectors.EVENT_WRITE:
            if message.outb:
                print(f"sending message to {message.addr[0]}")
                sent = self.socket.send(message.outb)  
                message.outb = message.outb[sent:]

    def handle_request(self,message):
        try:
            res = b''
            header = Request_Header(message)
            if header.code == REQ_CODE.REGISTER.value:
                format = f'<{header.payload_size}s'
                user_name = unpack_from(format,buffer=message,offset=HEADER_SIZE)[0].decode('utf-8').rstrip('\x00')
                res = self.controller.register(user_name)
                
            elif header.code == REQ_CODE.SEND_PUBLIC_KEY.value:
                format = f'<{NAME_SIZE}s{PUBLIC_KEY_SIZE}s'
                _user_name, public_key = unpack_from(format,buffer=message,offset=HEADER_SIZE)

                res = self.controller.send_key(header.client_id, public_key)

            elif header.code == REQ_CODE.SEND_FILE.value:
                format = f'<{CLIENT_ID_SIZE}sI{NAME_SIZE}s'
                _client_id, file_size, file_name = unpack_from(format,buffer=message,offset=HEADER_SIZE)
                encrypted_file = self.get_full_encrypted_file(message)

                file_name = file_name.decode('utf-8').rstrip('\x00')

                res = self.controller.receive_file(header.client_id, encrypted_file, file_name )

            elif header.code == REQ_CODE.CRC_VALID.value:
                file_name = self.get_file_name_from_crc_message(message)
                res = self.controller.verify_file(header.client_id,file_name)
            
            elif header.code == REQ_CODE.CRC_INVALID.value:
                return b''
                
            elif header.code == REQ_CODE.CRC_FAILED.value:
                file_name = self.get_file_name_from_crc_message(message)
                self.controller.invalidate_file(header.client_id,file_name)
                return b''
            else:
                raise Exception('Invalid code error')
            return res
        except Exception as error:
            print(error)
            return

    def get_file_name_from_crc_message(self, message):
        format = f'<{CLIENT_ID_SIZE}s{NAME_SIZE}s'
        _client_id, file_name = unpack_from(format,buffer=message,offset=HEADER_SIZE)
        return file_name

    def get_full_encrypted_file(self, message):
        file_content_start = HEADER_SIZE + NAME_SIZE + CLIENT_ID_SIZE + U_INT_SIZE
        encrypted_file = message[file_content_start:]
        if len(message) == MESSAGE_SIZE:
            message_suffix = self.__get_message_file()
            encrypted_file += message_suffix
        return encrypted_file
            
    def __get_message_file(self):
            message = b''
            while True:
                part = self.socket.recv(MESSAGE_SIZE)
                message += part
                if len(part) < MESSAGE_SIZE:
                    break
            return message            

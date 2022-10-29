import selectors
import types    
import socket

from Services import Services
from Database import Database
from struct import unpack_from

from Models import HEADER_SIZE, REQ_CODE, Request_Header
from Controller import Controller


HOST = "127.0.0.1"  
MESSAGE_SIZE = 1024
TIMEOUT = 0

class Server:
    def __init__(self,test=False):
        try:
            PORT = self.__get_port_from_file()
            db = Database()
            services = Services(db)
            self.controller = Controller(services)
            
            if test:
                return 
            self.__connection = Connection(PORT)
            self.__listen()

        except Exception as error:
            # TODO: change to res.error(....
            print(error)
        finally:
            if not test:
                self.__connection.selector.close()
    
    def __get_port_from_file(self):
        try:
            port_file = open("server/port.info", "r")
            PORT = int(port_file.readline())
            port_file.close()
        except FileNotFoundError:
            raise Exception('port file not found')
        return PORT

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
        socket = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            recv_data = socket.recv(MESSAGE_SIZE)
            if recv_data:
                data.outb = self.handle_request(recv_data)
            # else:
                # print(f"Closing connection to {data.addr}")
                # self.__connection.selector.unregister(socket)
                # socket.close()
        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print(f"Echoing {data.outb!r} to {data.addr}")
                sent = socket.send(data.outb)  # Should be ready to write
                data.outb = data.outb[sent:]

    
    def handle_request(self,data):
        header = Request_Header(data)
        if header.code == REQ_CODE.REGISTER.value:
            format = f'<{header.payload_size}s'
            user_name = unpack_from(format,buffer=data,offset=HEADER_SIZE)[0].decode('utf-8')
            res = self.controller.register(user_name)
            
        elif header.code == REQ_CODE.SEND_PUBLIC_KEY.value:
            pass
        elif header.code == REQ_CODE.SEND_FILE.value:
            pass
        elif header.code == REQ_CODE.CRC_FAILED.value:
            pass
        elif header.code == REQ_CODE.CRC_INVALID.value:
            pass
        elif header.code == REQ_CODE.CRC_VALID.value:
            pass
        else:
            raise Exception('Invalid code error')
        return res

class Connection:
    def __init__(self,PORT):
        self.selector = selectors.DefaultSelector()
        self.__server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__server_socket.bind((HOST, PORT))
        self.__server_socket.listen()
        print(f"server is listening on port {PORT}")
        self.__server_socket.setblocking(False)
        self.selector.register(self.__server_socket, selectors.EVENT_READ, data=None)

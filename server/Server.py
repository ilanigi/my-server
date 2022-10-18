import selectors
import types    
import socket

HOST = "127.0.0.1"  
MESSAGE_SIZE = 1024
TIMEOUT = 1

class Server:
    def __init__(self):
        try:
            PORT = self.__get_port_from_file()
            self.__init_listening(PORT)
            self.__listen()
        except Exception as error:
            print(error)
        finally:
            self.__selector.close()

    def __listen(self):
        while True:
            events = self.__selector.select(timeout=TIMEOUT)
            for key, mask in events:
                if key.data is None:
                    self.__accept_wrapper(key.fileobj)
                else:
                    self.__service_connection(key, mask)

    def __init_listening(self, PORT):
        self.__selector = selectors.DefaultSelector()
        self.__server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__server_socket.bind((HOST, PORT))
        self.__server_socket.listen()
        print(f"server is listening on port {PORT}")
        self.__server_socket.setblocking(False)
        self.__selector.register(self.__server_socket, selectors.EVENT_READ, data=None)

    def __get_port_from_file(self):
        try:
            port_file = open("server/port.info", "r")
            PORT = int(port_file.readline())
            port_file.close()
        except FileNotFoundError:
            raise Exception('port file not found')
        return PORT

    def __accept_wrapper(self,sock):
        conn, addr = sock.accept()  
        print(f"Accepted connection from {addr[0]}")
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.__selector.register(conn, events, data=data)

    def __service_connection(self, key, mask):
        socket = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            recv_data = socket.recv(MESSAGE_SIZE)
            if recv_data:
                data.outb += recv_data
            else:
                print(f"Closing connection to {data.addr}")
                self.__selector.unregister(socket)
                socket.close()
        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print(f"Echoing {data.outb!r} to {data.addr}")
                sent = socket.send(data.outb)  # Should be ready to write
                data.outb = data.outb[sent:]
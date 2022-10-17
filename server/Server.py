import selectors
import types    
import socket

HOST = "127.0.0.1"  
TIMEOUT = 1

class Server:
    def __init__(self):
        PORT = self.__get_port_from_file()
        self.__init_listening(PORT)
        try:
            self.__listen()
        except Exception as error:
            print(error)
        finally:
            self.selector.close()

    def __listen(self):
        while True:
            events = self.selector.select(timeout=TIMEOUT)
            for key, mask in events:
                if key.data is None:
                    self.__accept_wrapper(key.fileobj,self.selector)
                else:
                    self.__service_connection(key, mask,self.selector)

    def __init_listening(self, PORT):
        self.selector_ = selectors.DefaultSelector()
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((HOST, PORT))
        self.server_socket.listen()
        print(f"server is listening on port {PORT}")
        self.server_socket.setblocking(False)
        self.selector.register(self.server_socket, selectors.EVENT_READ, data=None)

    def __get_port_from_file(self):
        try:
            port_file = open("server/port.info", "r")
            PORT = int(port_file.readline())
            port_file.close()
        except FileNotFoundError:
            raise Exception('port file not found')
        return PORT

    def __accept_wrapper(self,sock):
        conn, addr = sock.accept()  # Should be ready to read
        print(f"Accepted connection from {addr[0]}")
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.selector.register(conn, events, data=data)

    def __service_connection(self, key, mask):
        sock = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            recv_data = sock.recv(1024)  # Should be ready to read
            if recv_data:
                data.outb += recv_data
            else:
                print(f"Closing connection to {data.addr}")
                self.selector.unregister(sock)
                sock.close()
        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print(f"Echoing {data.outb!r} to {data.addr}")
                sent = sock.send(data.outb)  # Should be ready to write
                data.outb = data.outb[sent:]
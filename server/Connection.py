import selectors
import socket

HOST = "127.0.0.1"  
DEFAULT_PORT = 1234


class Connection:
    """
    connection class handling the connection of the server to the socket, and getting and writing the messages
    """

    def __init__(self):
        """init the server connection"""
        PORT = self.__get_port_from_file()
        self.selector = selectors.DefaultSelector()
        self.__server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__server_socket.bind((HOST, PORT))
        self.__server_socket.listen()
        print(f"server is listening on port {PORT}")
        self.__server_socket.setblocking(False)
        self.selector.register(self.__server_socket, selectors.EVENT_READ, data=None)

    def __get_port_from_file(self):
            try:
                port_file = open("server/port.info", "r")
                PORT = int(port_file.readline())
                port_file.close()
            except FileNotFoundError:
                print('port.info file not found, using default port.')
                PORT = DEFAULT_PORT

            return PORT
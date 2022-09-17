# import selectors
# import socket

# sel = selectors.DefaultSelector()

# def accept(sock, msk):
#     conn, add = sock.accept()
#     # 
#     conn.setblockgking(False)
#     sel.register(conn,selectors.EVENT_READ,read)
    
import socket
HOST = "127.0.0.1"  


def main():

    try:
        port_file = open("port.info", "r")
        PORT = int(port_file.readline())
    except:
        print('file not found')    


    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.bind((HOST, PORT))
        client_socket.listen()
        conn, addr = client_socket.accept()
        with conn:
            print(f"Connected by {addr}")
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                conn.sendall(data)


if __name__ == '__main__':
    main()
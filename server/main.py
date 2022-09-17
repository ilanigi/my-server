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
        port_file = open("server/port.info", "r")
        PORT = int(port_file.readline())
        port_file.close()
    except:
        print('file not found')    


    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.bind((HOST, PORT))
        client_socket.listen()
        print("server ready.")
        conn, addr = client_socket.accept()
        print("client connected.")
        with conn:
            print(f"Connected by {addr}")
            while True:
                data = conn.recv(1024)
                print(data)


if __name__ == '__main__':
    main()
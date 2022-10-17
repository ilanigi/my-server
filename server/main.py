import selectors
import types    
import socket

HOST = "127.0.0.1"  
TIMEOUT = 1

def accept_wrapper(sock,selector):
    conn, addr = sock.accept()  # Should be ready to read
    print(f"Accepted connection from {addr[0]}")
    conn.setblocking(False)
    data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
    events = selectors.EVENT_READ | selectors.EVENT_WRITE
    selector.register(conn, events, data=data)

def service_connection(key, mask,selector):
    sock = key.fileobj
    data = key.data
    if mask & selectors.EVENT_READ:
        recv_data = sock.recv(1024)  # Should be ready to read
        if recv_data:
            data.outb += recv_data
        else:
            print(f"Closing connection to {data.addr}")
            selector.unregister(sock)
            sock.close()
    if mask & selectors.EVENT_WRITE:
        if data.outb:
            print(f"Echoing {data.outb!r} to {data.addr}")
            sent = sock.send(data.outb)  # Should be ready to write
            data.outb = data.outb[sent:]


def main():
    try:
        port_file = open("server/port.info", "r")
        PORT = int(port_file.readline())
        port_file.close()
    except:
        print('file not found')    

    selector = selectors.DefaultSelector()
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen()
        print("server is ready for connections")
        server_socket.setblocking(False)
        selector.register(server_socket, selectors.EVENT_READ, data=None)
        
        
        try:
            while True:
                events = selector.select(timeout=TIMEOUT)
                for key, mask in events:
                    if key.data is None:
                        accept_wrapper(key.fileobj,selector)
                    else:
                        service_connection(key, mask,selector)
        except Exception as e:
            print(e)
        finally:
            selector.close()



if __name__ == '__main__':
    main()
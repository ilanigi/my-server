# my-server

This is a simple client server project for secured file or message transfer using RSA and AES keys.

To activate is you can open the client (c++) in visual studio 
and the server (python) in visual studio code.

To run the code on visual studio - make sure you do the following:
- add boost library via nuget.
- add crypto++ library using guide in open university site.
- make sure all code source file are visible in 'filter' file explorer mode. 
This can be done be dragging the files from their folder in windows file explorer to the file explorer in visual studio.

Python server can run via python using main file or via vs code. I used python 3.11 but it can also run on python 3.7.

HOW DOES THE SERVER WORKS?
On init, server is building the database. If Db exist he use it. 
after that the services are contracting - which are al depended on the db.

Each request is getting a thread using selector, the header is parsed and a switch is separating the request to the right handler.
The handling is in controller file. This is a group of functions that handling service functions. 
The controller functions returning a compiled response which transfer back to the client, in case were the response is not empty.

HOW DOES THE CLIENT WORKS?
Each request is a static method that creating a new instance of services. 
One of this services is io service which init connection to the server. 
But the connection will fully establish when tre request will be send using io.send method.

One of this methods (send file) is making multiple requests to the server. This is way each request is connecting and disconnecting the socket and connection.io_service.

Technology:
Serve:
- sqlite3 for db.
- Crypto.cipher for cryptography handling.
- zlib for crc32 checksum.
- selectors for selectors

Client:
- boost for socket and crc32 checksum.
- crypto++ for cryptography handling.




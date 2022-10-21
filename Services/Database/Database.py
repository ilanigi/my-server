import sqlite3

import os.path as path
DB_FILE = "server.db"

class DB:
    def __init__(self):
        db_exist = path.isfile(DB_FILE)
        if not db_exist:
            new_file = open(DB_FILE, "x")
            
        connection = sqlite3.connect(DB_FILE)  
        self.curser = connection.cursor()

        if not db_exist:
            # TODO: add types
            self.curser.execute("CREATE TABLE clients(id, name, publicKey, lastSeen, AESKey)")
            self.curser.execute("CREATE TABLE files(userId, name, oathName, verified)")

    

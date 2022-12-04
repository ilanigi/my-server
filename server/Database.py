import sqlite3
import os.path as path
from Table import Table

DB_FILE = "server.db"

class Database:
    def __init__(self):
        db_exist = path.isfile(DB_FILE)
        if not db_exist:
            new_file = open(DB_FILE, "x")
            print("Database created")
        else:
            print("Database found")

        connection = sqlite3.connect(DB_FILE)
        curser = connection.cursor()

        self.clients = Table((connection, curser, db_exist), "clients", [("id", "VARCHAR PRIMARY KEY", bytes), (
            "name", "VARCHAR UNIQUE", str), ("publicKey", "VARCHAR", bytes), ("lastSeen", "VARCHAR", str), ("AESKey", "VARCHAR", bytes)])\

        self.files = Table((connection, curser, db_exist), "files", [("id", "VARCHAR", bytes), (
            "name", "VARCHAR", str), ("pathName", "VARCHAR PRIMARY KEY", str), ("verified", "INTEGER", bool)])
        

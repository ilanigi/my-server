import sqlite3
import os.path as path
from Table import Table
DB_FILE = "server.db"

class Database:
    def __init__(self):
        db_exist = path.isfile(DB_FILE)
        if not db_exist:
            new_file = open(DB_FILE, "x")
            print("Databse created")
        else:
            print("Database found")

        connection = sqlite3.connect(DB_FILE)
        curser = connection.cursor()

        self.users = Table((connection, curser, db_exist), "clients", [("id", "VARCHAR PRIMARY KEY", str), (
            "name", "VARCHAR UNIQUE", str), ("publicKey", "VARCHAR", str), ("lastSeen", "FLOAT", float), ("AESKey", "VARCHAR", str)])\

        self.files = Table((connection, curser, db_exist), "files", [("id", "VARCHAR PRIMARY KEY", str), (
            "name", "VARCHAR", str), ("pathName ", "VARCHAR UNIQUE", str), ("verified", "VARCHAR ", bool)])
        

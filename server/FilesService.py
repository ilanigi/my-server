from Database import Database
from os import path, makedirs, remove
from Checksum import crc32
import zlib

BULK_SIZE = 4096
forbidden_chars = ["..","\\","/"]
class Files:
    
    def __init__(self, db: Database):
        self.mem = {}
        self.db = db

    def add_file(self, client_id, file_name, file_content):
        BASE_PATH = "clients_files/"    
        
        if self.file_exist(file_name, client_id):
            raise Warning("File already exist for this user")
        
        for char in forbidden_chars:
            if char in file_name:
                raise Exception("invalid file name")
        
        file_path = BASE_PATH + f"{client_id.hex()}"
        
        if not path.exists(file_path):
            makedirs(file_path)
        
        file_path += f"/{file_name}"
        with open(file_path, "wb") as file:
            file.write(file_content)

        self.db.files.create(["id", "name", "pathName", "verified"], [
                             client_id, file_name, file_path, False])
        
        self.mem[file_path] = {'id':client_id, 'name': file_name}

        return file_path

    def file_exist(self, file_name, client_id):

        res = self.db.files.read(["id", "name"], [client_id, file_name])
        return res[2] if not res is None else False
    
    def remove_file(self,file_name, client_id):
        file_path = self.file_exist(file_name,client_id)
        
        self.db.files.delete(["id","name"],[client_id, file_name])
        
        if path.exists(file_path):
            remove(file_path)

        self.mem[file_path] = None
        

    def set_verify_true(self, file_name, client_id):
        self.db.files.update(
            (["id", "name"], [client_id, file_name]), (["verified"], [True]))
        
        return

    def check_sum(self,file_path):
        crc_value = 0
        with open(file_path, 'rb') as file:
            buffer = file.read(BULK_SIZE)
            while len(buffer) > 0:
                crc_value = zlib.crc32(buffer, crc_value)
                buffer = file.read(BULK_SIZE)
                
        return crc_value
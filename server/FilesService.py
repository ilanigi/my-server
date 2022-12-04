from Database import Database
from os import path, makedirs
from Checksum import crc32

class Files:
    BASE_PATH = "clients_files/"
    
    def __init__(self, db: Database):
        self.db = db

    def add_file(self, client_id, file_name, file_content):
        if self.file_exist(file_name, client_id):
            raise Warning("File already exist for this user")
        
        if ".." in file_name:
            raise Exception("invalid file name")
        
        file_path = self.BASE_PATH + f"{client_id.hex()}"
        
        if not path.exists(file_path):
            makedirs(file_path)
        
        file_path += f"/{file_name}"
        with open(file_path, "wb") as file:
            file.write(file_content)

        self.db.files.create(["id", "name", "pathName", "verified"], [
                             client_id, file_name, file_path, False])

    def file_exist(self, file_name, client_id):
        res = self.db.files.read(["id", "name"], [client_id, file_name])

        return not res is None

    def set_verify_true(self, file_name, client_id):

        self.db.clients.update(
            (["id", "name"], [client_id, file_name]), (["verified"], [True]))
        return

    def check_sum(self,file_path):
        BULK_SIZE = 4096
        with open(file_path, "rb") as file:
            digest = crc32()
            
            while buf := file.read(BULK_SIZE):
                digest.update(buf)
        
        return digest.digest()
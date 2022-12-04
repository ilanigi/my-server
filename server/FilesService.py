from Database import Database
from os import path, makedirs

class Files:
    def __init__(self, db: Database):
        self.db = db

    def add_file(self, client_id, file_name, file_content):
        if self.file_exist(file_name, client_id):
            raise Warning("File already exist for this user")
        
        if ".." in file_name:
            raise Exception("invalid file name")
        
        file_path = f"{client_id.hex()}"
        
        if not path.exists(file_path):
            makedirs(file_path)
        
        file_path += f"/{file_name}"
        with open(file_path, "w") as file:
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

    def check_sum(file):
        # crc_calculator = crc_calculator(Crc32.CRC32)
        # checksum = crc_calculator.calculate_checksum(file)
        return 'checksum'
        pass

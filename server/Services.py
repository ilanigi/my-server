from UsersService import Users
from SecretService import SecretService
from SendService import Send
from FilesService import Files

class Services:
    def __init__(self,db):
        self.users = Users(db)
        self.files = Files(db)
        self.secrets = SecretService()
        self.send = Send()

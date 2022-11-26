from UsersService import Clients
from SecretService import SecretService
from SendService import Send
from FilesService import Files

class Services:
    def __init__(self,db):
        self.users = Clients(db)
        self.files = Files(db)
        self.secrets = SecretService()
        self.send = Send()

from UsersService import Users
from SecretService import SecretService
from SendService import Send

class Services:
    def __init__(self,db):
        self.users = Users(db)
        self.secrets = SecretService()
        self.send = Send()

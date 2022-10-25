from UsersService import Users
from SecretService import SecretService

class Services:
    def __init__(self,db):
        self.users = Users(db)
        self.secrets = SecretService()

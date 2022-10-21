from Users import User

class Services:
    def __init__(self,db):
        self.users = User(db)

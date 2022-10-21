from Users import User
from Database import Database


class Services:
    def __init__(self):
        db = Database()
        self.users = User(db.curser)

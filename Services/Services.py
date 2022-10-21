from Services.Users import User
from Services.Database.Database import DB


class Services:
    def __init__(self):
        db = DB()
        self.users = User(db.curser)

from asyncio.windows_events import NULL
from msilib.schema import tables
import uuid
from datetime import datetime
from Database import Database


class User:
    def __init__(self, db:Database):
        self.db = db

    def user_exist(self, user_name:str):
        # TODO: add check by the book
        res = self.db.users.read(["name"],[user_name])
        
        user_exist = not res is None 
        
        if user_exist:
            user_id = res[0]
            now = datetime.now().timestamp()
            self.db.users.update((["id"], [user_id]),(["lastSeen"],[now]))
       
        return user_exist
        

    def save_new_to_db(self,user_name:str):
        user_id = str(uuid.uuid4())
        now = datetime.now().timestamp()

        self.db.users.create(["id", "name", "lastSeen"], [user_id, user_name, now])
        
    
    




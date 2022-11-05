import uuid
from datetime import datetime
from Database import Database

class Users:
    def __init__(self, db:Database):
        self.db = db

    def user_exist_by_name(self, user_name:str):
        # TODO: add check by the book
        res = self.db.users.read(["name"],[user_name])
        
        user_exist = not res is None 
        
        if user_exist:
            user_id = res[0]
            now = str(datetime.now())
            self.db.users.update((["id"], [user_id]),(["lastSeen"],[now]))
       
        return user_exist

    def user_exist_by_id(self, user_id:str):
        # TODO: add check by the book
        res = self.db.users.read(["id"],[user_id])
        
        user_exist = not res is None 
        
        if user_exist:
            user_id = res[0]
            now = str(datetime.now())
            self.db.users.update((["id"], [user_id]),(["lastSeen"],[now]))
       
        return user_exist
    
    def get_user_id_by_name(self,user_name):
        res = self.db.users.read(["name"],[user_name])
        if res is None:
            raise Exception("user not found")
        return res[0]

    def save_user_to_db(self, user_name:str):
        user_id = uuid.uuid4().bytes_le
        now = str(datetime.now())

        self.db.users.create(["id", "name", "lastSeen"], [user_id, user_name, now])
        return user_id

    def add_public_key(self, user_name:str,public_key:str ):    
        self.db.users.update((["name"],[user_name]),(["publicKey"],[public_key]))
    
    def add_AES_key(self,user_id:str,AESkey:str):
        self.db.users.update((["id"],[user_id]),(["AESKey"],[AESkey]))
    
        
    
    




from datetime import datetime
from Database import Database

class Clients:
    def __init__(self, db:Database):
        self.db = db
        self.mem = {}
    def __update_client_last_seen(self,client_id:str ):
            now = str(datetime.now())
            self.db.clients.update((["id"], [client_id]),(["lastSeen"],[now]))
            return
    
    def client_exist_by_name(self, client_name:str):
        # TODO: add check by the book
        res = self.db.clients.read(["name"],[client_name])
        
        client_exist = not res is None 
        
        if client_exist:
            client_id = res[0]
            self.__update_client_last_seen(client_id)
       
        return client_exist

    def client_exist_by_id(self, client_id:str):
        # TODO: add check by the book
        res = self.db.clients.read(["id"],[client_id])
        
        client_exist = not res is None 
        
        if client_exist:
            client_id = res[0]
            self.__update_client_last_seen(client_id)
       
        return client_exist
    
    def get_client_id_by_name(self,client_name):
        client = self.db.clients.read(["name"],[client_name])
        if client is None:
            raise Exception("client not found")
        return client[0]

    def save_client_to_db(self, client_name:str, client_id:bytes):
        now = str(datetime.now())
        self.db.clients.create(["id", "name", "lastSeen"], [client_id, client_name, now])
        
        self.mem[client_id.hex()] = {'name':client_name}

    def add_public_key(self, client_id:str,public_key:str ):    
        self.db.clients.update((["id"],[client_id]),(["publicKey"],[public_key]))
    
    def add_AES_key(self,client_id:str,AESkey:str):
        self.db.clients.update((["id"],[client_id]),(["AESKey"],[AESkey]))
    
    def get_AES_key(self,client_id:bytes):
        client = self.db.clients.read(["id"],[client_id])
        
        if client is None:
            raise Exception("client not found")
        
        if client[4] is None:
            raise Exception("client AES key not found")

        return client[4]


    
        
    
    




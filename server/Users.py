from asyncio.windows_events import NULL
from msilib.schema import tables
import uuid

from datetime import datetime
# clients(id INTEGER PRIMARY KEY, name VARCHAR UNIQUE, publicKey VARCHAR, lastSeen VARCHAR, AESKey VARCHAR)
class User:
    def __init__(self, curser):
        # self.__curser = curser

    def user_exist(self, user_name:str):
        # # add check by the book
        # res = self.__curser.execute(f"SELECT * FROM clients WHERE name='{user_name}'")
        # user_exist = not res.fetchone() is None
        
        # if user_exist:
            # now = datetime.now()
            # self.__curser.execute("""UPDATE clients SET lastSeen = '{now}' WHERE name='{user_name}'""".format(now=now,user_name=user_name))
       
        # return user_exist

    def save_new_to_db(self,user_name:str):
        user_id = str(uuid.uuid4())
        now = datetime.now()

        
        query = """INSERT INTO clients (id, name, lastSeen) VALUES ('{user_id}','{user_name}','{now}')""".format(user_id=user_id,user_name=user_name,now=now)


        self.__curser.execute(query)
        
    
    




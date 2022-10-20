from datetime import datetime
# TODO: create a service class 
class User:
    def __init__(self, curser):
        self.curser = curser

    def user_exist(self, user_name):
        # add check by the book
        res = self.curser.execute(f"SELECT * FROM clients WHERE name='{user_name}'")
        user_exist = not res.fetchone() is None
        
        if user_exist:
            now = datetime.now()
            self.curser.execute(f"UPDATE clients SET lastSeen = '{now}' WHERE name={user_name}")
            
        return user_exist
    
    




from Database import Database
class Files:
    def __init__(self, db:Database):
        self.db = db
    
    def add_file(self,client_id,file_name,file_content):
        path = client_id.hex() + "/" + file_name
        
        with open("path", "w") as file:
            file.write(file_content)

        self.db.files.create(["id","name","pathName","verified"],[client_id,file_name,path,False])
    
    def file_exist(self, file_name, client_id):
        res = self.db.files.read(["id","name"],[client_id, file_name])
        
        return not res is None 


    def verify_file(self,file_name, client_id):
        self.db.users.update((["id","name"],[client_id, file_name]),(["verified"],[True]))
        pass
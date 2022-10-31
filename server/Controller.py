from Services import Services


class Controller:
    def __init__(self, services:Services) -> None:
        self.services = services
        pass

    def register(self,user_name:str):
        # TODO: add check  - max user name size is 255 chars
        if self.services.users.user_exist_by_name(user_name):
            raise Exception('user is already exist')
            
        user_id = self.services.users.save_user_to_db(user_name)
        return self.services.send.ok.register(user_id)
    
    def send_key(self,user_id, public_key):
        if not self.services.users.user_exist_by_id(user_id):
            raise Exception('user not exist')

        self.services.users.add_public_key(user_id,public_key)
        AES_key = self.services.secrets.create_AES_key()
        




from Services import Services


class Controller:
    def __init__(self, services:Services) -> None:
        self.services = services
        pass

    def register(self,user_name:str):
        if self.services.users.user_exist(user_name):
            raise Exception('user is already exist')
            
        user_id = self.services.users.save_user_to_db(user_name)
        return self.services.send.ok.register(user_id)

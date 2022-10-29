from Services import Services


class Controller:
    def __init__(self, services:Services) -> None:
        self.services = services
        pass

    def register(self,userId:str):
        if self.services.users.user_exist(userId):
            raise Exception('user is already exist')
            
        self.services.users.save_user_to_db(userId)
        return b'200'

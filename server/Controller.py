from Services import Services
import uuid


class Controller:
    def __init__(self, services:Services) -> None:
        self.__services = services
        pass

    def register(self,user_name:str):
        # TODO: add check  - max user name size is 255 chars
        if self.__services.users.user_exist_by_name(user_name):
            raise Exception('user is already exist')
        user_id = uuid.uuid4().bytes_le
        self.__services.users.save_user_to_db(user_name,user_id)
        return self.__services.send.ok.register(user_id)
    
    def send_key(self,user_id, public_key):
        if not self.__services.users.user_exist_by_id(user_id):
            raise Exception('user not exist')

        self.__services.users.add_public_key(user_id,public_key)
        AES_key = self.__services.secrets.create_AES_key()
        print('AES_key is', AES_key.hex())
        self.__services.users.add_AES_key(user_id, AES_key)      
        encrypt_AES_key = self.__services.secrets.encrypt_AES_key(AES_key,public_key)
        print('encrypt_AES_key is', encrypt_AES_key.hex())
        return self.__services.send.ok.send_public_key(encrypt_AES_key)
        




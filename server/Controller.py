from Services import Services
import uuid
from Models import RES_CODE

class Controller:
    def __init__(self, services:Services) -> None:
        self.__services = services
        pass

    def register(self,user_name:str):
        # TODO: add check  - max user name size is 255 chars
        if self.__services.users.client_exist_by_name(user_name):
            raise Exception('user is already exist')
        client_id = uuid.uuid4().bytes_le
        print('client id is:',client_id.hex())
        self.__services.users.save_client_to_db(user_name, client_id)
        print("Registered user {} successfully".format(user_name))
        return self.__services.send.ok(RES_CODE.ACC_REGISTER.value, client_id)
    
    def send_key(self,user_id, public_key):
        if not self.__services.users.client_exist_by_id(user_id):
            raise Exception('user not exist')
        
        print('public_key is', public_key.hex())
        
        self.__services.users.add_public_key(user_id,public_key)
        AES_key = self.__services.secrets.create_AES_key()
        
        print('AES_key is', AES_key.hex())
        
        self.__services.users.add_AES_key(user_id, AES_key)      
        encrypt_AES_key = self.__services.secrets.encrypt_AES_key(AES_key,public_key)
        
        print('encrypt_AES_key is', encrypt_AES_key.hex())
        print("key exchanges successfully")

        return self.__services.send.ok(RES_CODE.ACC_PUBLIC_KEY.value,encrypt_AES_key)
    
    def receive_file(self,client_id:bytes, encrypted_file:bytes,file_name:str):
        if not self.__services.users.client_exist_by_id(client_id):
            raise Exception('user not exist')

        AES_key = self.__services.users.get_AES_key(client_id)
        file = self.__services.secrets.decrypt_file(AES_key,encrypted_file)
        
    
        self.__services.files.add_file(file,client_id,file)

    def verify_file(self, file_name, client_id):

        self.__services.files.set_verify(file_name, client_id, True)




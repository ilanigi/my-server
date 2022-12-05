from Services import Services
import uuid
from Models import RES_CODE
MAX_USER_NAME = 100
class Controller:
    """
    the controller class is the actual handling for each request, using the services
    """
    def __init__(self, services:Services) -> None:
        self.__services = services
        pass

    def register(self,user_name:str):
        """register a given user by name if user is not already exist"""
        try:
            if len(user_name) > MAX_USER_NAME:
                raise Exception('user name is too long')

            if self.__services.users.client_exist_by_name(user_name):
                raise Exception('user is already exist')

            client_id = uuid.uuid4().bytes_le
            print('client id is:',client_id.hex())
            self.__services.users.save_client_to_db(user_name, client_id)
            print("Registered user {} successfully".format(user_name))
            return self.__services.send.message(RES_CODE.ACC_REGISTER.value, client_id)
        except Exception as error:
            print(error)
            return self.__services.send.message(RES_CODE.REGISTER_FAILED.value,(None))
    
    def send_key(self,client_id, public_key):
        """
        getting public key from user and, encrypting a new AES key and sending it back
        """
        try:
            if not self.__services.users.client_exist_by_id(client_id):
                raise Exception('user not exist')
            
            print('public_key is',public_key.hex()[:26] + '...' + public_key.hex()[-3:])
            
            self.__services.users.add_public_key(client_id, public_key)
            AES_key = self.__services.secrets.create_AES_key()
            
            print('AES_key is', AES_key.hex())
            
            self.__services.users.add_AES_key(client_id, AES_key)      
            encrypt_AES_key = self.__services.secrets.encrypt_AES_key(AES_key,public_key)
            
            print('encrypt_AES_key is', encrypt_AES_key.hex()[:26] + "..." + encrypt_AES_key.hex()[-3:] )
            print("key exchanges successfully")

            return self.__services.send.message(RES_CODE.ACC_PUBLIC_KEY.value, (client_id, encrypt_AES_key))
        except Exception as error:
            print(error)
            return b''
    
    def receive_file(self,client_id:bytes, encrypted_file:bytes,file_name:str):
        """
        receiving a file from user, decrypting it, calculating it's checksum using c2c32 and adding it to db and memory
        """
        try:
            if not self.__services.users.client_exist_by_id(client_id):
                raise Exception('user not exist')

            AES_key = self.__services.users.get_AES_key(client_id)
            file = self.__services.secrets.decrypt_file(AES_key,encrypted_file)
            file_path = self.__services.files.add_file(client_id,file_name,file)
            
            print(f'file {file_name} added to db and memory')

            checksum = self.__services.files.check_sum(file_path)
            print("checksum:", checksum)
            return self.__services.send.message(RES_CODE.ACC_FILE.value, (client_id, len(file), file_name, checksum))
        except Exception as error:
            print(error)
            return b''

    def verify_file(self, client_id, file_name ):
        """this means client has approved that file send successfully by checksum. set file validation to true in db"""
        try:
            if not self.__services.users.client_exist_by_id(client_id):
                raise Exception('user not exist')
            
            if not self.__services.files.file_exist(file_name,client_id):
                raise Exception('file not exist')
            
            self.__services.files.set_verify_true(file_name, client_id)

            return self.__services.send.message(RES_CODE.ACC_MESSAGE.value)
        except Exception as error:
            print(error)
            return b''
    
    def remove_file(self,client_id,file_name):
        """
        remove file in case it is not send well
        """
        try:
            if not self.__services.users.client_exist_by_id(client_id):
                raise Exception('user not exist')
            
            if not self.__services.files.file_exist(file_name,client_id):
                raise Exception('file not exist') 
            
            self.__services.files.remove_file(file_name,client_id)
            print(f"file {file_name} removed")
            return b''

        except Exception as error:
            print(error)
            return b''
        
        



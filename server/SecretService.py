from Crypto.Cipher import PKCS1_OAEP, AES
from Crypto.Random import get_random_bytes 
from Crypto.PublicKey.RSA import import_key
from Crypto.Util.Padding import unpad

class SecretService:
    AES_KEY_SIZE = 16
    iv = bytearray(AES_KEY_SIZE)
    
    def __init__(self) -> None:
        pass

    def create_AES_key(self):
        return get_random_bytes(self.AES_KEY_SIZE)
    
    def encrypt_AES_key(self, AES_key, public_key):
        compiled_public_key = import_key(public_key)
        cipher_obj = PKCS1_OAEP.new(compiled_public_key)
        encrypt_AES_key = cipher_obj.encrypt(AES_key)
        return encrypt_AES_key

    def decrypt_file(self,AES_key, decrypted_file):
        cipher = AES.new(AES_key, AES.MODE_CBC, self.iv)
        file = unpad(cipher.decrypt(decrypted_file), AES.block_size)
        return file


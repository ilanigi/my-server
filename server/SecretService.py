from Crypto.Cipher import PKCS1_OAEP, AES
from os import urandom
from Crypto.PublicKey.RSA import import_key
from Crypto.Hash import SHA1

class SecretService:
    def __init__(self) -> None:
        pass

    def create_AES_key(self):
        return urandom(16)
    
    def encrypt_AES_key(self, AES_key, public_key):
        compiled_public_key = import_key(public_key)
        cipher_obj = PKCS1_OAEP.new(compiled_public_key)
        encrypt_AES_key = cipher_obj.encrypt(AES_key)
        return encrypt_AES_key

    def decrypt_file(self,AES_key, file):
        cipher = AES.new(AES_key, AES.MODE_EAX,iv=0)
        encrypt_public_key, _ = cipher.encrypt_and_digest(file)
            # file = AESkey.decrypt(file)
        return file

    def check_sum(file):
        # crc_calculator = CrcCalculator(Crc32.CRC32)
        # checksum = crc_calculator.calculate_checksum(file)
        # return checksum
        pass

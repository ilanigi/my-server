from Crypto.Cipher import AES,PKCS1_OAEP
import os
# from crc import CrcCalculator, Crc32

class SecretService:
    def __init__(self) -> None:
        pass

    def create_AES_key(self):
        return os.urandom(16)
    
    def encrypt_AES_key(self, AES_key, public_key):
        cipher_rsa = PKCS1_OAEP.new(public_key)
        encrypt_public_key = cipher_rsa.encrypt(AES_key)
        return encrypt_public_key

    def decrypt_file(AESkey, file):
         # cipher = AES.new(AES_key, AES.MODE_EAX)
        # encrypt_public_key, _ = cipher.encrypt_and_digest(public_key)
        file = AESkey.decrypt(file)
        return file

    def check_sum(file):
        # crc_calculator = CrcCalculator(Crc32.CRC32)
        # checksum = crc_calculator.calculate_checksum(file)
        # return checksum
        pass

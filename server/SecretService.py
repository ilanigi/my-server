from Crypto.Cipher import AES
# from crc import CrcCalculator, Crc32

class SecretService:
    def __init__(self) -> None:
        pass

    def create_AES_key(public_key:str):
        AESkey = AES.new(public_key, AES.MODE_EAX)
        return AESkey
    
    def encrypt_AES_key(self, AES_key, pub):
        pass

    def decrypt_file(AESkey, file):
        file = AESkey.decrypt(file)
        return file

    def check_sum(file):
        # crc_calculator = CrcCalculator(Crc32.CRC32)
        # checksum = crc_calculator.calculate_checksum(file)
        # return checksum
        pass

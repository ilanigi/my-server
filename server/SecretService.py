from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

class SecretService:
    def __init__(self) -> None:
        pass

    def create_AES_key(public_key:str):
        cipher = AES.new(public_key, AES.MODE_EAX)
        return cipher

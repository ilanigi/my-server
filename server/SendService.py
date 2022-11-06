from Models import RES_CODE, Response


class Send:
    def __init__(self) -> None:
        self.ok = OK()
        self.failed = Failed()

class Failed:
    def __init__(self) -> None:
        pass
    
    def general(self,error):
        res = Response(400, error)
        return res.compiled

class OK:
    def __init__(self) -> None:
        pass

    def register(self, user_id: str):
        res = Response(RES_CODE.ACC_REGISTER.value, user_id)
        return res.compiled

    def send_public_key(self, encrypt_public_key):
        res = Response(RES_CODE.ACC_PUBLIC_KEY.value, encrypt_public_key)
        return res.compiled

    
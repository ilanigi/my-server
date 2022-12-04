from Models import RES_CODE, Response, ACC_REGISTER, ACC_PUBLIC_KEY, ACC_FILE, ACC_MESSAGE, REGISTER_FAILED


class Send:
    def __init__(self) -> None:
        pass


    def message(self, code, payload = None) -> None:
        if code == RES_CODE.ACC_REGISTER.value:
            res = ACC_REGISTER(payload) 
        elif code == RES_CODE.ACC_PUBLIC_KEY.value:
            res = ACC_PUBLIC_KEY(*payload)
        elif code == RES_CODE.ACC_FILE.value:
            res = ACC_FILE(*payload)
        elif code == RES_CODE.ACC_MESSAGE.value:
            res = ACC_MESSAGE()
        elif code == RES_CODE.REGISTER_FAILED.value:
            res = REGISTER_FAILED()
        return res.compiled   


    
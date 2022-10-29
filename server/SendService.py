from Models import RES_CODE, Response


class Send:
    def __init__(self) -> None:
        self.ok = OK()
        pass

class OK:
    def __init__(self) -> None:
        pass

    def register(self, user_id: str):
        res = Response(RES_CODE.ACC_REGISTER.value, user_id)
        return res.compiled


    
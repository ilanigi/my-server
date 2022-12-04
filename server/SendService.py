from Models import RES_CODE, Response


class Send:
    def __init__(self) -> None:
        pass
    
    def general(self,error):
        res = Response(400, error)
        return res.compiled

    def ok(self, code, payload) -> None:
        res = Response(code, payload)
        return res.compiled   


    
from re import L
from sqlite3 import Connection, Cursor
from typing import List, Tuple

from Database import Database

class Table:
    def __init__(self,cradeniales:Tuple, name:str, columns: List[Tuple[str,str,type]]):
        self.connection: Connection
        self.curser: Cursor
        self.connection, self.curser, db_exist = cradeniales
        self.name = name
        self.types = {}
        for column in columns:
            self.types[column[0]] = column[2]

        # if table is not in the db - create one
        if not db_exist:
            query = self.create_new_table_query(columns)
            self.curser.execute(query)
            self.connection.commit()
        
        def execute(self,method):
            query = method()
            self.curser.execute(query)
            self.connection.commit()

        @execute
        def create(self,columns:List, values:List):
            if not len(columns) == len(values):
                raise Exception("values amount are not equel to columns to insert to amount" )
            
            # check types
            for i in range(len(columns)):
                if not self.types[columns[i]] == type(values[i]):
                    raise Exception(f"value {columns[i]} is not of type {self.types[columns[i]]}" )
            
            query = """INSERT INTO {table} ({columns}) VALUES ({values})""".format(table=self.name,columns=columns, values=values)
            return query
            
        def read():
            pass
        def update():
            pass
        def deelete():
            pass

    def create_new_table_query(self, columns):
        str_colomns = ""
        for column in columns:
           str_colomns +=  f"{column[0]} {column[1]},"
        column[-1] = ""
        
        query = """CREATE TABLE {name} ({str_colomns})""".format(name=self.name,str_colomns=str_colomns)
        return query
    
    


            

    


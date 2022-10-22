from sqlite3 import Connection, Cursor
from typing import List, Tuple


class Table:
    def __init__(self, cradeniales: Tuple, name: str, columns: List[Tuple[str, str, type]]):
        self.__connection: Connection
        self.__curser: Cursor
        self.__connection, self.__curser, db_exist = cradeniales
        self.__name = name

        self.__types = {}
        for column in columns:
            self.__types[column[0]] = column[2]

        # if table is not in the db - create one
        if not db_exist:
            query = self.__create_new_table_query(columns)
            self.__curser.execute(query)
            self.__connection.commit()

    def __check_input(self, columns: List, values: List):
        if not len(columns) == len(values):
            raise Exception(
                "values amount are not equel to columns to insert to amount")

        # check types
        for i in range(len(columns)):
            if not self.__types[columns[i]] == type(values[i]):
                raise Exception("""value {val} is not of type {des}. insted {given} was given""".format(
                    val=columns[i], des=self.__types[columns[i]], given=type(values[i])))
        
      
    def __init_input(self,raw_columns, raw_values):
        self.__check_input(raw_columns, raw_values)
        columns = self.__list_to_string(raw_columns)
        values = self.__list_to_string(raw_values,True,raw_columns)
        return columns, values


    def create(self, raw_columns: List, raw_values: List):
        columns, values = self.__init_input(raw_columns, raw_values)
        
        query = """INSERT INTO {table} ({columns}) VALUES ({values});""".format(
            table=self.__name, columns=columns, values=values)
        
        self.__execute(query)

    def read():
        pass

    def update():
        pass

    def deelete():
        pass

    def __execute(self, query):
        self.__curser.execute(query)
        self.__connection.commit()

    def __create_new_table_query(self, columns):
        str_colomns = ""
        for column in columns:
            str_colomns += f"{column[0]} {column[1]}, "

        str_colomns = str_colomns[0:-2] 
        query = """CREATE TABLE {name} ({str_colomns})""".format(
            name=self.__name, str_colomns=str_colomns)
        return query

    def __list_to_string(self,val_list:List, values:bool = False, columns:List =[] ):
        to_ret = ""
        for i in range(len(val_list)):
            # adding quotation marks to string values
            if values and self.__types[columns[i]] == str:
                to_ret += f"'{val_list[i]}', "
            else:
                to_ret += f"{val_list[i]}, "
        to_ret = to_ret[0:-2] + " "
        return to_ret
from sqlite3 import Connection, Cursor
from typing import List, Tuple

from Utils import list_to_comma_string, lists_to_string_with_q_mark


class Table:
    def __init__(self, credentials: Tuple, name: str, columns: List[Tuple[str, str, type]]):
        self.__connection: Connection
        self.__curser: Cursor
        self.__connection, self.__curser, db_exist = credentials
        self.__name = name

        self.__types = {}
        for column in columns:
            self.__types[column[0]] = column[2]

        # if table is not in the db - create one
        if not db_exist:
            query = self.__create_new_table_query(columns)
            self.__curser.execute(query)
            self.__connection.commit()

    def create(self, raw_columns: List, raw_values: List):
        self.__check_input(raw_columns, raw_values)
        columns = list_to_comma_string(raw_columns)
        values_as_q_marks = list_to_comma_string(raw_values, True)

        query = """INSERT INTO {table} ({columns}) VALUES ({values});""".format(
            table=self.__name, columns=columns, values=values_as_q_marks)

        self.__execute_write(query,tuple(raw_values))

    def read(self, raw_columns: List, raw_values: List):
        self.__check_input(raw_columns, raw_values)
        filter_with_q_marks = lists_to_string_with_q_mark(raw_columns)

        query = """SELECT * FROM {table} WHERE {filters}""".format(
            table=self.__name, filters=filter_with_q_marks)

        return self.__execute_read(query,tuple(raw_values))
    
    def update(self, filter: Tuple, mutations: Tuple):
        raw_filter_columns, raw_filter_values = filter
        raw_mutations_columns, raw_mutations_values = mutations

        self.__check_input(raw_filter_columns, raw_filter_values)
        self.__check_input(raw_mutations_columns, raw_mutations_values)

        mutation_with_q_mark = lists_to_string_with_q_mark(
            raw_mutations_columns,values=True)
        filter_with_q_mark = lists_to_string_with_q_mark(raw_filter_columns)

        query = """UPDATE {table} SET {mutation} WHERE {filter}""".format(
            table=self.__name, mutation=mutation_with_q_mark, filter=filter_with_q_mark)

        concatenated_values =  raw_mutations_values + raw_filter_values
        self.__execute_write(query,tuple(concatenated_values))

    def delete(self, raw_columns, raw_values):
        self.__check_input(raw_columns, raw_values)
        filter_with_q_mark = lists_to_string_with_q_mark(raw_columns)

        query = """DELETE FROM {table} WHERE {filter}""".format(table=self.__name, filter=filter_with_q_mark)
        
        self.__execute_write(query,tuple(raw_values))

    def __execute_read(self, query:str, values:Tuple):
        res = self.__curser.execute(query,values)
        return res.fetchone()

    def __execute_write(self, query,values=None):
        res = self.__curser.execute(query,values)
        com = self.__connection.commit()
    
    def __check_input(self, columns: List, values: List):
        if not len(columns) == len(values):
            raise Exception(
                "values amount are not equal to columns to insert to amount")

        # check types
        for i in range(len(columns)):
            if self.__types[columns[i]] == None:
                raise Exception(
                    f"type {columns[i]} does not exist in this table")

            if not self.__types[columns[i]] == type(values[i]):
                raise Exception("""value {val} is not of type {des}. instead {given} was given""".format(
                    val=columns[i], des=self.__types[columns[i]], given=type(values[i])))

    def __create_new_table_query(self, columns):
        str_columns = ""
        for column in columns:
            str_columns += f"{column[0]} {column[1]}, "

        str_columns = str_columns[0:-2]
        query = """CREATE TABLE {name} ({str_columns})""".format(
            name=self.__name, str_columns=str_columns)
        return query

    

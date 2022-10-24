from typing import List


def lists_to_string_with_q_mark(raw_columns: List, values:bool=False):
        to_ret = ""
        for i in range(len(raw_columns)):
            if values:
                to_ret += f"{raw_columns[i]}=?, "
            else:
                to_ret += f"{raw_columns[i]}=? AND"

        to_ret = to_ret[0:-2] if values else to_ret[0:-4]
        return to_ret    

def list_to_comma_string( val_list: List, values: bool = False):
        to_ret = ""
        for i in range(len(val_list)):
            # adding quotation marks to string values
            if values:
                to_ret += "?, "
            else:
                to_ret += f"{val_list[i]}, "

        to_ret = to_ret[0:-2] + " "
        return to_ret
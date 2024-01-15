import os
import re
from pathlib import Path

def file_not_empty(file_name):
        
        script_dir = Path(__file__).resolve().parent
        file_path = script_dir / file_name

        print(os.path.exists(file_path))
        print((os.path.getsize(file_path) > 0))
        return os.path.exists(file_path) and (os.path.getsize(file_path) > 0)




def find_largest_number(input_string):
    # extract all numbers from the string
    numbers = re.findall(r'\d+', input_string)

    # return None if no number has been found
    if not numbers:
        return None

    # else: return the largest
    max_number = max(map(int, numbers))
    return max_number

def split(SubTaskString):
        SubTaskNumber = find_largest_number(SubTaskString)
        
      
        initial_split = SubTaskString.split("1. ")
        SubTaskArray = [initial_split[1]]

        for splitter in range (2, SubTaskNumber+1):
                splitting = SubTaskArray.pop().split(str(splitter)+". ")
                        
                SubTaskArray.append(splitting[0])
                SubTaskArray.append(splitting[1])
                        
                splitter += 1

        return SubTaskArray
       
               



# ToDo
def test(file_name):
        return True
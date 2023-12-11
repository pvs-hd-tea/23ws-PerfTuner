import os
from pathlib import Path

def file_not_empty(file_name):
        
        script_dir = Path(__file__).resolve().parent
        file_path = script_dir / file_name

        print(os.path.exists(file_path))
        print((os.path.getsize(file_path) > 0))
        return os.path.exists(file_path) and (os.path.getsize(file_path) > 0)

# ToDo
def test(file_name):
        return True
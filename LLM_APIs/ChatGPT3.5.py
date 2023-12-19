# Test how to call the API of ChatGPT 3.5

from dotenv import load_dotenv
from pathlib import Path
from openai import OpenAI

load_dotenv()

input = "example1.cpp"
input1 = "library1.cc"
output = "exampleOpt.cpp"

script_dir = Path(__file__).resolve().parent
file_path = script_dir / input
file_in = open(file_path,"r")
promt = file_in.read()
file_in.close()
file_path = script_dir / input1
file_in = open(file_path,"r")
library = file_in.read()
file_in.close()


environment = "double A[n][n+1], n is a multiple of B, B is chosen by the user"
#"double A[n][n+1], n is a multiple of B, B is chosen by the user"
#"const int M = 8; n is multiple of M."


file_path = script_dir / output
file_out = open(file_path,"w")


client = OpenAI()

#Be aware that loading into the AVX registers from a n-dimensional matrix is only possible \
#if the loop variable only appears in the last field.\
#You will be given some extra information about the \
#environment of the function after the key word 'environment:'.\
#You will also be given some extra information that might help you with your task. \
#Your task is to optimize that function using AVX intrinsics. \
#Determine whether the function contains similar instances to the snippets of the library \
#that you could use to optimize the function."},
#You will also be given two c++ snippets after the key word 'library'. \
#Both snippets do the same, however the second snippet makes use of AVX intrinsics. \
#You will also be given some extra information about the \
#environment of the function after the key word 'environment:'.\


response = client.chat.completions.create(
    model="gpt-3.5-turbo",
    temperature=1.0,
    messages=[
    {"role": "system", "content": "You will be given a c++ function after the key word 'function:'\
     Make a list of subtasks of the function."},
    {"role": "user", "content": "function: " + promt},
    #{"role": "user", "content": "environment: " + environment},
    #{"role": "user", "content": "library: " + library},
  ]
)
generated_text = response.choices[0].message.content
file_out.write(generated_text)
print(generated_text)

file_out.close()

